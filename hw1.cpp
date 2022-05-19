#include <QCoreApplication>
#include "hw1.h"
#include <iostream>
#include <fstream>

using namespace std;

Img::Img(){
    srcImg = new int[1000*1000];
    width = 1000;
    height = 1000;
}

Img::Img(int w, int h){
    width = w;
    height = h;
    srcImg = new int[w*h];
}

Img::Img(const int* src, int w, int h){
    srcImg = new int[w*h];
    for (int i = 0; i < w*h; i++){
        srcImg[i] = src[i];
    }
    width = w;
    height = h;
}

Img::~Img(){
    delete[] srcImg;
}

ImageProcess::ImageProcess(){}

ImageProcess::ImageProcess(int w, int h){
    this->srcImg = new Img(w, h);
    this->processedImg = new Img(w, h);
    this->mask = new Img (3, 3);
}

ImageProcess::ImageProcess(const Img* img){
    this->srcImg = new Img(img->srcImg, img->width, img->height);
    this->processedImg = new Img(img->width, img->height);
    this->mask = new Img (3, 3);
}

ImageProcess::ImageProcess(const char* fileName){
    int w, h;
    ifstream file(fileName);
    file >> w;
    file >> h;
    file.close();
    this->srcImg = new Img(w, h);
    this->processedImg = new Img(w, h);
    this->mask = new Img(3, 3);
    ImageProcess::loadImgFromFile(fileName, 1);
}

ImageProcess::~ImageProcess(){
    delete srcImg;
    delete processedImg;
    delete mask;
}

int ImageProcess::updateMask(const Img& mask){
    this->mask->srcImg = mask.srcImg;
    this->mask->x_c = mask.x_c;
    this->mask->y_c = mask.y_c;

    cout << "Mask was updated" << endl;
}

int ImageProcess::updateSrcImg(){
    for (int i = 0; i < this->srcImg->width*this->srcImg->height; i++){
        this->srcImg->srcImg[i] = this->processedImg->srcImg[i];
    }
    cout << "SrcImage was updated" << endl;
}

int ImageProcess::dilatation(int srcImg){
    if (srcImg == 1){
        for (int i = 0; i < this->srcImg->width*this->srcImg->height; i++){
            this->processedImg->srcImg[i] = this->srcImg->srcImg[i];
        }
    }

    int h = this->srcImg->height;
    int w = this->srcImg->width;
    int cy = this->mask->y_c;
    int cx = this->mask->x_c;

    int arr[(h+4)*(w+4)];
    for (int i = 0; i < h+4; i++){
        for (int j = 0; j < w+4; j++){
            if (i<2 || j<2 || i>=h+2 || j>=w+2){
                arr[i*(w+4) + j] = 0;
            }
            else{
                arr[i*(w+4) + j] = this->processedImg->srcImg[(i-2)*w + (j-2)];
            }
        }
    }

    for (int i = 2; i < h+2; i++){
        for (int j = 2; j < w+2; j++){
            if (arr[i*(w+4) + j] == 1){
                for (int m = 0; m < 3; m++){
                    for (int n = 0; n < 3; n++){
                        if (this->mask->srcImg[m*3 + n] == 1){
                                this->processedImg->srcImg[(i-2+m-cy)*w + j-2+n-cx] = 1;

                        }
                    }
                }
            }

        }
    }

    cout << "dilatation was completed" << endl;
}

int ImageProcess::erosion(int srcImg){
    if (srcImg == 1){
        for (int i = 0; i < this->srcImg->width*this->srcImg->height; i++){
            this->processedImg->srcImg[i] = this->srcImg->srcImg[i];
        }
    }

    int h = this->srcImg->height;
    int w = this->srcImg->width;
    int cy = this->mask->y_c;
    int cx = this->mask->x_c;

    int arr[(h+4)*(w+4)];
    for (int i = 0; i < h+4; i++){
        for (int j = 0; j < w+4; j++){
            if (i<2 || j<2 || i>=h+2 || j>=w+2){
                arr[i*w + j] = 1;
            }
            else{
                arr[i*w + j] = this->processedImg->srcImg[(i-2)*w + (j-2)];
            }
        }
    }
    for (int i = 2; i < h+2; i++){
        for (int j = 2; j < w+2; j++){
            if (arr[i*w + j] == 1){
                int k = 0;
                for (int m = 0; m < 3; m++){
                    for (int n = 0; n < 3; n++){
                        if (arr[(i-cy+m)*w + j-cx+n] >= this->mask->srcImg[m*3 + n]){
                                k += 1;
                        }
                    }
                }
                if (k < 9){
                    this->processedImg->srcImg[(i-2)*w + j-2] = 0;
                }
            }
        }
    }
    cout << "Erosion was completed" << endl;
}

int ImageProcess::loadImgFromFile(const char* fileName, int format){
    ifstream file(fileName);
    cout << "File was opened" << endl;
    int w, h;
    file >> w;
    file >> h;
    file.get();

    char str[h*(w+1)];

    for (int i = 0; i < h; i++){
        for (int j = 0; j < w+1; j++){
            char ch = file.get();
            str[i*(w+1) + j] = ch;
        }
  }

    file.close();
    cout << "File was closed" << endl;

    int v = 0;

    for(int i = 0; i < h; i++){
        for(int j = 0; j < w+1; j++){
            if (str[i*(w+1)+j] == '0' /*0x30*/){
                this->srcImg->srcImg[v] = 0;
                this->processedImg->srcImg[v] = 0;
                v++;
            }
            if (str[i*(w+1)+j] == '1' /*0x31*/){
                this->srcImg->srcImg[v] = 1;
                this->processedImg->srcImg[v] = 1;
                v++;
            }
        }
    }
    cout << "File was loaded" << endl;
}

int ImageProcess::saveImgToFile(const char* fileName, int format){
    ofstream file (fileName);
    file << this->srcImg->width << "\t" << this->srcImg->height << endl;
    if (format == 1){
    for(int i = 0; i < this->srcImg->height; i++){
        for(int j = 0; j < this->srcImg->width; j++){
            file << this->srcImg->srcImg[i*this->srcImg->width+j];
        }
        file << endl;
    }
    }
    else {
        for(int i = 0; i < this->srcImg->height; i++){
            for(int j = 0; j < this->srcImg->width; j++){
                file << this->srcImg->srcImg[i*this->srcImg->width+j];
            }
        }
    }
    file.close();
    cout << "File was saved" << endl;
}

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    Img _mask(3,3);
    _mask.srcImg[0] = 0;_mask.srcImg[1] = 1;_mask.srcImg[2] = 0;
    _mask.srcImg[3] = 1;_mask.srcImg[4] = 1;_mask.srcImg[5] = 1;
    _mask.srcImg[6] = 0;_mask.srcImg[7] = 1;_mask.srcImg[8] = 0;
    _mask.x_c = 1; _mask.y_c = 1;

    class ImageProcess b("gost56.txt");
    b.updateMask(_mask);
    for(int i = 0; i < 5; i++){
        b.erosion(0);
    }

    b.updateSrcImg();
    b.saveImgToFile("erosiondynamicmask56.txt", 1);
    cout << "erosion has saved" << endl;

    class ImageProcess c("gost56.txt");
    c.updateMask(_mask);
    for(int i = 0; i < 5; i++){
        c.dilatation(0);
    }
    c.updateSrcImg();
    c.saveImgToFile("dilatationdynamicmask56.txt", 1);
    cout << "Dil saved" << endl;

    return a.exec();
}
