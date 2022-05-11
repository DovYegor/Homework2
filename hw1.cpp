#include <QCoreApplication>
#include "hw1.h"
#include <iostream>
#include <fstream>

using namespace std;

/*struct Img {
    int* srcImg;
    int width;
    int height;

class ImageProcess {
private :
    Img* srcImg;
    Img* processedImg;
    Img* mask;
*/

Img::Img(){
    srcImg = new int[1000*1000];
    width = 1000;
    height = 1000;
}

Img::Img(int w, int h){
    srcImg = new int[w*h];
    width = w;
    height = h;
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

ImageProcess::ImageProcess(int w, int h){/* выделяется память для картинки размера w*h */
    this->srcImg = new Img(w, h);
    this->processedImg = new Img(w, h);
    this->mask = new Img (3, 3);
}

ImageProcess::ImageProcess(const Img* img){/* выделяется память для картинки размера w*h и копируется картинка*/
    this->srcImg = new Img(img->srcImg, img->width, img->height);
    this->processedImg = new Img(img->width, img->height);
    this->mask = new Img (3, 3);
}

ImageProcess::ImageProcess(const char* fileName){/* выделяется память для картинки копируется картинка из файла*/
    int w, h;
    ifstream file(fileName);
    file >> w;
    file >> h;
    file.close();
    //cout << w << h << endl << endl;
    this->srcImg = new Img(w, h);
    this->processedImg = new Img(w, h);
    this->mask = new Img(3, 3);
    ImageProcess::loadImgFromFile(fileName);
}

ImageProcess::~ImageProcess(){
    delete srcImg;
    delete processedImg;
    delete mask;
}

int ImageProcess::updateMask(const Img& mask){/*задание маски*/
    this->mask->srcImg = mask.srcImg;

 /*   mask.srcImg[0] = 0; mask.srcImg[1] = 1; mask.srcImg[2] = 0;
    mask.srcImg[3] = 1; mask.srcImg[4] = 1; mask.srcImg[5] = 1;
    mask.srcImg[6] = 0; mask.srcImg[7] = 1; mask.srcImg[8] = 0; */

    cout << "Mask was updated" << endl;
}

int ImageProcess::updateSrcImg(){/*перезаписать исходную картинку картинкой, которая была получена в результате дилотации и/или эрозии*/
    for (int i = 0; i < this->srcImg->width*this->srcImg->height; i++){
        this->srcImg->srcImg[i] = this->processedImg->srcImg[i];
    }
    cout << "SrcImage was updated" << endl;
}

int ImageProcess::dilotation(){/*дилотация картинки, результат записать в processedImg*/
    for (int i = 0; i < this->srcImg->width*this->srcImg->height; i++){
        this->processedImg->srcImg[i] = this->srcImg->srcImg[i];
    }

    /*this->mask->srcImg[0] = 0; this->mask->srcImg[1] = 1; this->mask->srcImg[2] = 0;
    this->mask->srcImg[3] = 1; this->mask->srcImg[4] = 1; this->mask->srcImg[5] = 1;
    this->mask->srcImg[6] = 0; this->mask->srcImg[7] = 1; this->mask->srcImg[8] = 0; */

    for (int i = 0; i < this->srcImg->height; i++){
        for (int j = 0; j < this->srcImg->width; j++){
            if (this->srcImg->srcImg[i*this->srcImg->width + j] == 1  /*this->mask->srcImg[4]*/){
                for (int m = 0; m < 3; m++){
                    for (int n = 0; n < 3; n++){
                        if (this->mask->srcImg[m*3 + n] == 1){
                            if (i-1+m >= 0 & j-1+n >= 0 & i-1+m < this->srcImg->height & j-1+n < this->srcImg->width){
                                this->processedImg->srcImg[(i-1+m)*this->srcImg->width + j-1+n] = 1;
                            }
                        }
                    }
                }
            }
        }
    }

    cout << "Dilotation was completed" << endl;
}

int ImageProcess::erosion(){/*эрозия картинки, результат записать в processedImg*/
    for (int i = 0; i < this->srcImg->width*this->srcImg->height; i++){
        this->processedImg->srcImg[i] = this->srcImg->srcImg[i];
    }

    /*this->mask->srcImg[0] = 0; this->mask->srcImg[1] = 1; this->mask->srcImg[2] = 0;
    this->mask->srcImg[3] = 1; this->mask->srcImg[4] = 1; this->mask->srcImg[5] = 1;
    this->mask->srcImg[6] = 0; this->mask->srcImg[7] = 1; this->mask->srcImg[8] = 0; */

    for (int i = 0; i < this->srcImg->height; i++){
        for (int j = 0; j < this->srcImg->width; j++){
            if (this->srcImg->srcImg[i*this->srcImg->width + j] == this->mask->srcImg[4]){
                int k = 0;
                for (int m = 0; m < 3; m++){
                    for (int n = 0; n < 3; n++){
                        if (i-1+m >= 0 & j-1+n >= 0 & i-1+m < this->srcImg->height & j-1+n < this->srcImg->width){
                            if (this->srcImg->srcImg[(i-1+m)*this->srcImg->width + j-1+n] >= this->mask->srcImg[m*3 + n]){
                                k += 1;
                            //  cout << k << "\t" << this->srcImg->srcImg[(i-1+m)*this->srcImg->width + j-1+n] << endl;
                            }
                        }
                    }
                }
                if (k < 9){
                    this->processedImg->srcImg[i*this->srcImg->width + j] = 0;
                }
            }
        }
    }
/*
    for (int i = 0; i < this->srcImg->width*this->srcImg->height; i++){
        cout << this->srcImg->srcImg[i];
    }

    for (int i = 0; i < this->srcImg->width*this->srcImg->height; i++){
        cout << this->processedImg->srcImg[i];
    }
*/
    cout << "Erosion was completed" << endl;
}

int ImageProcess::loadImgFromFile(const char* fileName, int format){
    ifstream file(fileName);
    int w, h;
    file >> w;
    file >> h;
    char str[h][w+1];
    for (int i = 0; i < h; i++){
        file.getline(str[i], sizeof(str[i]));
    }
    file.close();

    for(int i = 1; i < h+1; i++){
        for(int j = 0; j < w; j++){
            if (str[i][j] == 0x30){
                this->srcImg->srcImg[(i-1)*w+j] = 0;
                this->processedImg->srcImg[(i-1)*w+j] = 0;
            }
            else{
                this->srcImg->srcImg[(i-1)*w+j] = 1;
                this->processedImg->srcImg[(i-1)*w+j] = 1;
            }
        }
    }
    cout << "File was loaded" << endl;
}

int ImageProcess::saveImgToFile(const char* fileName, int format){
    ofstream file (fileName);
    file << this->srcImg->width << "\t" << this->srcImg->height << endl;
    for(int i = 0; i < this->srcImg->height; i++){
        for(int j = 0; j < this->srcImg->width; j++){
            file << this->srcImg->srcImg[i*this->srcImg->width+j];
        }
        file << endl;
    }
    file.close();
    cout << "File was saved" << endl;
}

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    Img _mask(3,3);
    _mask.srcImg[0] = 1;_mask.srcImg[1] = 1;_mask.srcImg[2] = 1;
    _mask.srcImg[3] = 1;_mask.srcImg[4] = 1;_mask.srcImg[5] = 1;
    _mask.srcImg[6] = 1;_mask.srcImg[7] = 1;_mask.srcImg[8] = 1;

    class ImageProcess b("gost56.txt");
    b.updateMask(_mask);
    for(int i = 0; i < 10; i++){
        b.erosion();
        b.updateSrcImg();
    }
    b.saveImgToFile("erosion500600rectanglemask10.txt", 1);

    class ImageProcess c("gost56.txt");
    c.updateMask(_mask);
    for(int i = 0; i < 10; i++){
        c.dilotation();
        c.updateSrcImg();
    }
  //  cout << "Poka rabotaet" << endl;

    c.saveImgToFile("dilotation500600rectanglemask10.txt", 1);

    return a.exec();
}
