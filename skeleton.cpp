#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>

#include <iostream>
#include <array>
#include <math.h>

int counter;

int main(int argc, char **argv) {
	using namespace std;
	using namespace cv;

	int choice;
	int choice2;
	
	
	Mat image;
	Mat modified_image;
	Mat modified_image2;
	
	int unsharp [9] = { 1,1,1,
						1,-8,1,
						1,1,1 };

	int sobel_x[9] = { -1,0,1,
					   -2,0,2,
					   -1,0,1 };

	int sobel_y[9] = { -1,-2,-1,
					   0,0,0,
				       1,2,1 };
	int counter = 0;
	double r;
	double sigma;
	double half;
	double firstPart, secondPart, ePow;
	
	double log7[49];
	
	/*for (int x = 0; x < 7; x++) {
		for (int y = 0; y < 7; y++){
			half = 7 / 2;
			sigma = 1.4;
			r = sqrt((x - half) * (x - half) + (y - half) * (y - half));

			log7[counter] = ((r*r - 2 * sigma*sigma) / pow(sigma, 4))*exp(-r*r / (2 * sigma*sigma));

			counter++;
		}
	}*/

	for (int x = 0; x < 7; x++) {
		for (int y = 0; y < 7; y++) {
			half = 7 / 2;
			sigma = 1.4;
			r = sqrt((x - half) * (x - half) + (y - half) * (y - half));

			log7[counter] = ((r*r - 2 * sigma*sigma) / pow(sigma, 4))*exp(-r*r / (2 * sigma*sigma));

			counter++;
		}
	}
	/*for (int i = 0; i < 49; i++) {
		cout <<i<<" "<< (int)log7[i] << endl;
	}*/


	int log11[121];

	for (int x = 0; x < 11; x++) {
		for (int y = 0; y < 11; y++) {
			half = 11 / 2;
			sigma = 5;
			r = sqrt((x - half) * (x - half) + (y - half) * (y - half));

			log11[counter] = 300 * ((r*r - 2 * sigma*sigma) / pow(sigma, 4))*exp(-r*r / (2 * sigma*sigma));

			counter++;
		}
	}


	//for (int i = 0; i < 49; i++) cout <<i<<" "<< log7[i] << endl;

	int count;
	int sum;
	int sum2;
	double total;


	const char *filename = "../../../TestingImages3/ant_gray.bmp";
	const char *filename2 = "../../../TestingImages3/basel_gray.bmp";

	cout << "Type 1 for Ant image, 2 for Basel image" << endl;
	cin >> choice;

	if (choice == 1) {
		image = imread(filename, CV_LOAD_IMAGE_GRAYSCALE);
		modified_image = imread(filename, CV_LOAD_IMAGE_GRAYSCALE);
	}
	else if (choice == 2) {
		image = imread(filename2, CV_LOAD_IMAGE_GRAYSCALE);
		modified_image = imread(filename2, CV_LOAD_IMAGE_GRAYSCALE);
	}
	else cout << "Wrong input!" << endl;

	cout << "Type 1 for Unsharpening, 2 for Sobel filter, 3 for 7x7 sigma=1.4 LoG filter, 4 for 11x11 sigma=5 LoG filter" << endl;
	cin >> choice2;

	Mat *image_p = &image;
	
	//Get rows and columns size of image
	int row = image_p->rows;
	int col = image_p->cols;
	
	//Unsharpening
	if (choice2 == 1) {
		for (int i = 1; i <= row - 2; i++) {
			for (int j = 1; j <= col - 2; j++) {
				count = 0;
				for (int m = i - 1; m <= i + 1; m++) {
					for (int n = j - 1; n <= j + 1; n++) {
						sum += image.at<uchar>(m, n)*unsharp[count];
						count++;
					}
				}
				modified_image.at<uchar>(i, j) = sum / 9;
				sum = 0;
			}
		}



		for (int i = 0; i < row; i++) {
			for (int j = 0; j < col; j++) {
				modified_image.at<uchar>(i, j) += image.at<uchar>(i, j);
			}
		}

	}

	else if (choice2 == 2) {

		//Sobel

		for (int i = 1; i <= row - 2; i++) {
			for (int j = 1; j <= col - 2; j++) {
				count = 0;
				for (int m = i - 1; m <= i + 1; m++) {
					for (int n = j - 1; n <= j + 1; n++) {
						sum += image.at<uchar>(m, n)*sobel_x[count] / 8;
						sum2 += image.at<uchar>(m, n)*sobel_y[count] / 8;
						total = sqrt(pow((double)sum, 2) + pow((double)sum2, 2));
						count++;
					}
				}
				if (total > 255) total = 255;
				if (total < 0) total = 0;
				modified_image.at<uchar>(i, j) = total;
				sum = 0;
				sum2 = 0;
			}
		}
	}

	else if (choice2 == 3) {

		//LOG size 7
		
		for (int i = 4; i <= row - 6; i++) {
			for (int j = 4; j <= col - 6; j++) {
				count = 0;
				for (int m = i - 3; m <= i + 3; m++) {
					for (int n = j - 3; n <= j + 3; n++) {
						sum += image.at<uchar>(m, n)*log7[count];
						count++;
					}
				}

				modified_image.at<uchar>(i, j) = sum;
				sum = 0;
			}
		}
		
	}

	else if (choice2 == 4) {
		//LOG size 11
		
		for (int i = 6; i <= row - 10; i++) {
			for (int j = 6; j <= col - 10; j++) {
				count = 0;
				for (int m = i - 5; m <= i + 5; m++) {
					for (int n = j - 5; n <= j + 5; n++) {
						sum += image.at<uchar>(m, n)*log11[count];
						count++;
					}
				}
				modified_image.at<uchar>(i, j) = sum;
				sum = 0;
			}
		}
		
	}

	else cout << "Wrong input" << endl;
	//Canny Edge Detection



	imshow("Image", image);
	imshow("Modified Image", modified_image);

	//Display loop
	bool loop = true;
	while (loop) {

		switch (cvWaitKey(15)) {
		case 27:  //Exit display loop if ESC is pressed
			loop = false;
			break;
		default:
			break;
		}
	}


}


