//===========================================================================================================
//= �{�����g�H ����  �A 2015/09�t��ק�                                                                     =
//= �Y�ɰʧ@�ѧO�G�ѧO���ʧ@�O�]���B�^�ˡB���_�ӡB���U�B���U�B�s�y�^�C                                      =
//=                                                                                                         =
//===========================================================================================================
//�q�L�s��Ķ��̪�20�V�����[��ƨӽT�w�G�Y���M��ߪ���l��m�]�D�n�O���ס^�H�ζ^�˪��H�ȰѼơC2014�~4��16��C
#include "Header.h";


int main(int argc, char **argv)
{
#pragma region InitialNite And Ni
	// Initial NiTE
	if (NiTE::initialize() != STATUS_OK)
	{
		cerr << "NiTE initial error" << endl;
		system("PAUSE");
		return -1;
	}

	if (openni::OpenNI::initialize() != openni::STATUS_OK){
		cerr << "NiTE initial error" << endl;
		system("PAUSE");
		return -1;
	}
#pragma endregion


#pragma region GetAllDevice
	openni::Array<openni::DeviceInfo> deviceInfoList;
	openni::OpenNI::enumerateDevices(&deviceInfoList);
	cout << endl;
	cout << "Device numbers: " << deviceInfoList.getSize() << endl;

	vector<CDevice> vDevices;
	for (int i = 0; i < deviceInfoList.getSize(); ++i)
	{
		cout << "Device " << i << "\n";
		const openni::DeviceInfo& rDevInfo = deviceInfoList[i];

		cout << " " << rDevInfo.getName() << " by " << rDevInfo.getVendor() << "\n";
		cout << " PID: " << rDevInfo.getUsbProductId() << "\n";
		cout << " VID: " << rDevInfo.getUsbVendorId() << "\n";
		cout << " URI: " << rDevInfo.getUri() << endl;

		// initialize
		CDevice mDevWin(i, deviceInfoList[i].getUri());
		vDevices.push_back(mDevWin);
	}
#pragma endregion


	// start
	while (true)
	{
		for (vector<CDevice>::iterator itdev = vDevices.begin(); itdev != vDevices.end(); ++itdev){
			// get user frame
			UserTrackerFrameRef	mUserFrame;
			if (itdev->pUserTracker->readFrame(&mUserFrame) == nite::STATUS_OK)
			{
				// get depth data and convert to OpenCV format
				openni::VideoFrameRef vfDepthFrame = mUserFrame.getDepthFrame();
				const cv::Mat mImageDepth(vfDepthFrame.getHeight(), vfDepthFrame.getWidth(), CV_16UC1, const_cast<void*>(vfDepthFrame.getData()));
				// re-map depth data [0,Max] to [0,255]
				cv::Mat mScaledDepth;
				mImageDepth.convertTo(mScaledDepth, CV_8U, 255.0 / 10000);

				// convert gray-scale to color
				cv::Mat mImageBGR;
				cv::cvtColor(mScaledDepth, mImageBGR, CV_GRAY2BGR);

				// get users data
				const nite::Array<UserData>& aUsers = mUserFrame.getUsers();
				for (int i = 0; i < aUsers.getSize(); ++i)
				{
					const UserData& rUser = aUsers[i];

					// check user status
					if (rUser.isNew())
					{
						cout << "New User [" << rUser.getId() << "] found." << endl;
						//  start tracking for new user
						itdev->pUserTracker->setSkeletonSmoothingFactor(0.65); //���Ʊ���C
						itdev->pUserTracker->startSkeletonTracking(rUser.getId());
					}
					else if (rUser.isLost())
					{
						cout << "User [" << rUser.getId() << "] lost." << endl;
					}

					if (rUser.isVisible())
					{
						// get user skeleton
						const Skeleton& rSkeleton = rUser.getSkeleton();
						if (rSkeleton.getState() == SKELETON_TRACKED)
						{
#pragma region DrawSkeletonLineAndCircle
							// build joints array
							nite::SkeletonJoint aJoints[15];
							aJoints[0] = rSkeleton.getJoint(JOINT_HEAD);
							aJoints[1] = rSkeleton.getJoint(JOINT_NECK);
							aJoints[2] = rSkeleton.getJoint(JOINT_LEFT_SHOULDER);
							aJoints[3] = rSkeleton.getJoint(JOINT_RIGHT_SHOULDER);
							aJoints[4] = rSkeleton.getJoint(JOINT_LEFT_ELBOW);
							aJoints[5] = rSkeleton.getJoint(JOINT_RIGHT_ELBOW);
							aJoints[6] = rSkeleton.getJoint(JOINT_LEFT_HAND);
							aJoints[7] = rSkeleton.getJoint(JOINT_RIGHT_HAND);
							aJoints[8] = rSkeleton.getJoint(JOINT_TORSO);
							aJoints[9] = rSkeleton.getJoint(JOINT_LEFT_HIP);
							aJoints[10] = rSkeleton.getJoint(JOINT_RIGHT_HIP);
							aJoints[11] = rSkeleton.getJoint(JOINT_LEFT_KNEE);
							aJoints[12] = rSkeleton.getJoint(JOINT_RIGHT_KNEE);
							aJoints[13] = rSkeleton.getJoint(JOINT_LEFT_FOOT);
							aJoints[14] = rSkeleton.getJoint(JOINT_RIGHT_FOOT);

							// convert joint position to image
							cv::Point2f aPoint[15];
							for (int s = 0; s < 15; ++s)
							{
								const Point3f& rPos = aJoints[s].getPosition();
								itdev->pUserTracker->convertJointCoordinatesToDepth(rPos.x, rPos.y, rPos.z, &(aPoint[s].x), &(aPoint[s].y));
							}

							// draw line
							cv::line(mImageBGR, aPoint[0], aPoint[1], cv::Scalar(255, 0, 0), 3);
							cv::line(mImageBGR, aPoint[1], aPoint[2], cv::Scalar(255, 0, 0), 3);
							cv::line(mImageBGR, aPoint[1], aPoint[3], cv::Scalar(255, 0, 0), 3);
							cv::line(mImageBGR, aPoint[2], aPoint[4], cv::Scalar(255, 0, 0), 3);
							cv::line(mImageBGR, aPoint[3], aPoint[5], cv::Scalar(255, 0, 0), 3);
							cv::line(mImageBGR, aPoint[4], aPoint[6], cv::Scalar(255, 0, 0), 3);
							cv::line(mImageBGR, aPoint[5], aPoint[7], cv::Scalar(255, 0, 0), 3);
							cv::line(mImageBGR, aPoint[1], aPoint[8], cv::Scalar(255, 0, 0), 3);
							cv::line(mImageBGR, aPoint[8], aPoint[9], cv::Scalar(255, 0, 0), 3);
							cv::line(mImageBGR, aPoint[8], aPoint[10], cv::Scalar(255, 0, 0), 3);
							cv::line(mImageBGR, aPoint[9], aPoint[11], cv::Scalar(255, 0, 0), 3);
							cv::line(mImageBGR, aPoint[10], aPoint[12], cv::Scalar(255, 0, 0), 3);
							cv::line(mImageBGR, aPoint[11], aPoint[13], cv::Scalar(255, 0, 0), 3);
							cv::line(mImageBGR, aPoint[12], aPoint[14], cv::Scalar(255, 0, 0), 3);

							// draw joint
							for (int s = 0; s < 15; ++s)
							{
								if (aJoints[s].getPositionConfidence() > 0.5)
									cv::circle(mImageBGR, aPoint[s], 3, cv::Scalar(0, 0, 255), 2);
								else
									cv::circle(mImageBGR, aPoint[s], 3, cv::Scalar(0, 255, 0), 2);
							}

							cv::putText(mImageBGR, "USER" + to_string(rUser.getId()), aPoint[0], cv::FONT_HERSHEY_DUPLEX, 0.5, cv::Scalar(255, 255, 255));

#pragma endregion

							//�����l��
							if (itdev->detectFlag == 0)//�Ĥ@������
							{
#pragma region ��l��Algorithm
								itdev->DSTimes++;
								if (itdev->DSTimes == 1)
								{
									itdev->fheadf.x = aJoints[0].getPosition().x;//���߮ɪ��Y����m�F
									itdev->fheadf.y = aJoints[0].getPosition().y;
									itdev->fheadf.z = aJoints[0].getPosition().z;

									itdev->ftorsof.x = aJoints[8].getPosition().x;//���߮ɪ���ߦ�m�F
									itdev->ftorsof.y = aJoints[8].getPosition().y;
									itdev->ftorsof.z = aJoints[8].getPosition().z;

									itdev->lefthand.x = aJoints[6].getPosition().x;
									itdev->lefthand.y = aJoints[6].getPosition().y;
									itdev->lefthand.z = aJoints[6].getPosition().z;

									itdev->righthand.x = aJoints[7].getPosition().x;
									itdev->righthand.y = aJoints[7].getPosition().y;
									itdev->righthand.z = aJoints[7].getPosition().z;
									//���k��Z��*0.55�C ������n��0.55?
									itdev->armLen = sqrt((itdev->lefthand.x - itdev->righthand.x)*(itdev->lefthand.x - itdev->righthand.x) +
										(itdev->lefthand.y - itdev->righthand.y)*(itdev->lefthand.y - itdev->righthand.y) +
										(itdev->lefthand.z - itdev->righthand.z)*(itdev->lefthand.z - itdev->righthand.z))*0.55;
								}
								else if (itdev->DSTimes > 1 && itdev->DSTimes <= 200)	//�������Ʃ�200���H��
								{
									itdev->fheadf.x = itdev->fheadf.x + aJoints[0].getPosition().x;//���߮ɪ��Y����m(�֥[)�F
									itdev->fheadf.y = itdev->fheadf.y + aJoints[0].getPosition().y;
									itdev->fheadf.z = itdev->fheadf.z + aJoints[0].getPosition().z;

									itdev->ftorsof.x = itdev->ftorsof.x + aJoints[8].getPosition().x;//���߮ɪ���ߦ�m(�֥[)�F
									itdev->ftorsof.y = itdev->ftorsof.y + aJoints[8].getPosition().y;
									itdev->ftorsof.z = itdev->ftorsof.z + aJoints[8].getPosition().z;

									itdev->lefthand.x = aJoints[6].getPosition().x;
									itdev->lefthand.y = aJoints[6].getPosition().y;
									itdev->lefthand.z = aJoints[6].getPosition().z;

									itdev->righthand.x = aJoints[7].getPosition().x;
									itdev->righthand.y = aJoints[7].getPosition().y;
									itdev->righthand.z = aJoints[7].getPosition().z;
									//?????
									int cc = sqrt((itdev->lefthand.x - itdev->righthand.x)*(itdev->lefthand.x - itdev->righthand.x) +
										(itdev->lefthand.y - itdev->righthand.y)*(itdev->lefthand.y - itdev->righthand.y) +
										(itdev->lefthand.z - itdev->righthand.z)*(itdev->lefthand.z - itdev->righthand.z))*0.55;

									//	cout << "���դU�����G" << cc << endl;
									itdev->armLen = itdev->armLen + cc;
								}
								if (itdev->DSTimes >= 200)//�p���Y�B����l�ȡB�^�˻֭�
								{
									itdev->detectFlag = 1;
									itdev->fhead_Ini.x = itdev->fheadf.x / itdev->DSTimes;
									itdev->fhead_Ini.y = itdev->fheadf.y / itdev->DSTimes;
									itdev->fhead_Ini.z = itdev->fheadf.z / itdev->DSTimes;

									itdev->ftorso_Ini.x = itdev->ftorsof.x / itdev->DSTimes;
									itdev->ftorso_Ini.y = itdev->ftorsof.y / itdev->DSTimes;
									itdev->ftorso_Ini.z = itdev->ftorsof.z / itdev->DSTimes;

									itdev->fallDownThreshold = ((int)((itdev->armLen / itdev->DSTimes) / 100) * 100);

									cout << "�^�˪��H�ȡG" << itdev->fallDownThreshold << endl;

									//�p���Y ����e����
									itdev->fhead.x = aJoints[JOINT_HEAD].getPosition().x;
									itdev->fhead.y = aJoints[JOINT_HEAD].getPosition().y;
									itdev->fhead.z = aJoints[JOINT_HEAD].getPosition().z;
									itdev->fhead_Pre = itdev->fhead;

									itdev->ftorso.x = aJoints[JOINT_TORSO].getPosition().x;
									itdev->ftorso.y = aJoints[JOINT_TORSO].getPosition().y;
									itdev->ftorso.z = aJoints[JOINT_TORSO].getPosition().z;
									itdev->ftorso_Pre = itdev->ftorso;

									cout << "��" << itdev->DSTimes << "��" << "���Y��h�G" << itdev->fhead.y << endl;
								}
#pragma endregion
							}
							else
							{
								if (itdev->cishu == 8)
								{
									itdev->cishu = 0;
#pragma region MainAlgorithm 
									itdev->fhead.x = aJoints[0].getPosition().x;
									itdev->fhead.y = aJoints[0].getPosition().y;
									itdev->fhead.z = aJoints[0].getPosition().z;

									itdev->ftorso.x = aJoints[8].getPosition().x;
									itdev->ftorso.y = aJoints[8].getPosition().y;
									itdev->ftorso.z = aJoints[8].getPosition().z;

									itdev->neck.x = aJoints[1].getPosition().x;
									itdev->neck.y = aJoints[1].getPosition().y;
									itdev->neck.z = aJoints[1].getPosition().z;

									itdev->event_Now = eventDetect(itdev->fhead_Pre, itdev->ftorso_Pre, itdev->fhead, itdev->ftorso, itdev->fhead_Ini, itdev->ftorso_Ini, itdev->neck, itdev->fallDownThreshold);//(�e�@�V,��@�V,��l�V)


#pragma region �έp�ɶ��A�̮ɶ��ˬd�ʧ@�O�_���T�A�o�Ϳ��~�ȧ@��ܡA��{���޿�S���v�T
									//�έp�ɶ�
									if (itdev->event_Now == INCIDENT_SIT_DOWN)
										itdev->zuo = clock();
									if (itdev->event_Now == INCIDENT_FALLDOWN)
										itdev->die = clock();
									if (itdev->event_Now == INCIDENT_SIT_UP)
										itdev->zql = clock();
									if (itdev->event_Now == INCIDENT_STANDUP)
										itdev->zanql = clock();

									//����ܥX�i�઺���~��T�B�S����ڼv�T
									if (itdev->event_Now == INCIDENT_FALLDOWN && difftime(itdev->zuo, itdev->firsttime) > 0 && difftime(itdev->zuo_Pre, itdev->zuo) != 0)
									{
										automendG(itdev->event_Now, difftime(itdev->die, itdev->zuo));//�Y"�^��"�P"���U"�ƥ�j��10�� �h���ĵ�i
										itdev->zuo_Pre = itdev->zuo;
									}

									if (itdev->event_Now == INCIDENT_STANDUP && difftime(itdev->zql, itdev->firsttime) > 0 && difftime(itdev->zql_Pre, itdev->zql) != 0)
									{
										automendG(itdev->event_Now, difftime(itdev->zanql, itdev->zql));////�Y"���_��"�P"���_��"�ƥ�j��10�� �h���ĵ�i
										itdev->zql_Pre = itdev->zql;
									}
#pragma endregion


#pragma region �Ω�ոժ��A������ܥX�ƥ󰻴����G�A��{���޿�S���v�T�C
									if (itdev->event_Pre != itdev->event_Now)
									{
										if (itdev->event_Now == INCIDENT_NONE)
											cout << "�S������ƥ�o�͡I" << endl;
										if (itdev->event_Now == INCIDENT_STOP)
											cout << "�o�ͪ��ƥ�: " << "����" << endl;
										if (itdev->event_Now == INCIDENT_WALK)
											cout << "�o�ͪ��ƥ�: " << "��" << endl;
										if (itdev->event_Now == INCIDENT_SIT_UP)
											cout << "�o�ͪ��ƥ�: " << "���_��" << endl;
										if (itdev->event_Now == INCIDENT_STANDUP)
											cout << "�o�ͪ��ƥ�: " << "���_��" << endl;
										if (itdev->event_Now == INCIDENT_FALLDOWN)
											cout << "�o�ͪ��ƥ�: " << "�^��" << endl;
										if (itdev->event_Now == INCIDENT_SIT_DOWN)
											cout << "�o�ͪ��ƥ�: " << "���U" << endl;
										if (itdev->event_Now == INCIDENT_LAYDOWN)
											cout << "�o�ͪ��ƥ�: " << "���U" << endl;
									}
									itdev->event_Pre = itdev->event_Now;
#pragma endregion


									if (itdev->weizhi == 0)
									{
										itdev->nextState = firststate(itdev->event_Now);
										if (itdev->nextState != 0)
											itdev->weizhi = 1;
										displayState(itdev->nextState, itdev->nowState,itdev->deviceID);
										itdev->nowState = itdev->nextState;
									}
									else if (itdev->weizhi == 1)
									{
										if (itdev->event_Now != 0)
										{
											if (unnormalzhenc(itdev->event_Now, itdev->nowState) == 1);
											{
												itdev->weizhi = 0;
											}
											itdev->nextState = stateTransfer(itdev->event_Now, itdev->nowState);
										}
										displayState(itdev->nextState, itdev->nowState, itdev->deviceID);
										itdev->nowState = itdev->nextState;
									}
									itdev->fhead_Pre = itdev->fhead;
									itdev->ftorso_Pre = itdev->ftorso;
#pragma endregion
								}
								itdev->cishu++;
							}
						}
					}
				}

				// show image
				cv::imshow(itdev->sWindow.c_str(), mImageBGR);
				mUserFrame.release();
			}
			else
			{
				cerr << "Can't get user frame" << endl;
			}
		}
		// check keyboard
		if (cv::waitKey(1) == 'q')
			break;
	}

	// stop
	for (vector<CDevice>::iterator itDev = vDevices.begin(); itDev != vDevices.end(); ++itDev){
		itDev->pUserTracker->destroy();
		delete itDev->pUserTracker;
		itDev->pDevice->close();
		delete itDev->pDevice;
	}

	cv::destroyAllWindows();
	NiTE::shutdown();
	openni::OpenNI::shutdown();
	system("PAUSE");

	return 0;
}
