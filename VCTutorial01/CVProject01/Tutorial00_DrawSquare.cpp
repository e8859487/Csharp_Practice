#include "Header.h"


void tutorial00(){

	const int FRAMESIZE = 600;
	IplImage *image = cvCreateImage(cvSize(FRAMESIZE, FRAMESIZE), 8, 3);
	cvNamedWindow("����", 1);
	cvResizeWindow("����", FRAMESIZE, FRAMESIZE);
	cvSet(image, cvScalar(244, 244, 244));

	int x = 50, y = 50;
	int width = 50;
	int squareNum = 10;	//��l��Ƽƶq
	int squareSize = width * squareNum;	//�`�e/��
	CvScalar Color = cvScalar(0, 0, 0);	//����C��

	/*	�e�u	*/
	for (int i = 0; i <= squareNum; i++){
		cvLine(image, cvPoint(x + width * i, y), cvPoint(x + width * i, y + squareSize), Color, 3);
	}

	for (int i = 0; i <= squareNum; i++){
		cvLine(image, cvPoint(x, y + width * i), cvPoint(x + squareSize, y + width * i), Color, 3);
	}


	/*	�W�C��	*/
	const int offset = 5; //�񺡼аO�I�첾
	int oddFlag = 0;	//�򰸼ƿ�{�X��
	for (int i = 1; i <= squareNum; i++)
	{
		for (int j = 1; j <= squareNum; j++){
			if (j%2==oddFlag)
				cvFloodFill(image, cvPoint(x + j * width - offset, y - offset + i * width), Color);
		}
		oddFlag++;
		oddFlag = oddFlag % 2;
	}

	cvShowImage("����", image);

	cvWaitKey(0);
}