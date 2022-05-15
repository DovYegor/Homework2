struct Img {
    int* srcImg;
    int width;
    int height;

    int x_c;
    int y_c;

    Img();
    Img(int w, int h);
    Img(const int* src, int w, int h);
    ~Img();
};

class ImageProcess {
private :
    Img* srcImg;
    Img* processedImg;
    Img* mask;
    //здесь можно объ€вл€ть другие переменные и функции
public :
    //здесь нельз€ объ€вл€ть функции и переменные
    /**/
    ImageProcess();

    /* выдел€етс€ пам€ть дл€ картинки размера w*h */
    ImageProcess(int w, int h);

    /* выдел€етс€ пам€ть дл€ картинки размера w*h и копируетс€ картинка*/
    ImageProcess(const Img* img);

    /* выдел€етс€ пам€ть дл€ картинки копируетс€ картинка из файла*/
    ImageProcess(const char* fileName);

    ~ImageProcess();

    /*задание маски*/
    int updateMask(const Img& mask);

    /*перезаписать исходную картинку картинкой, котора€ была получена в результате дилотации и/или эрозии*/
    int updateSrcImg();

    /*дилотаци€ картинки, результат записать в processedImg*/
    int dilotation();

    /*эрози€ картинки, результат записать в processedImg*/
    int erosion();

    /*
        загрузить/сохранить из файла с именем fileName картинку
        input		:	fileName - им€ файла
                        format - формат файла 	0 - последовательность 0 и 1, без перехода на новую строку
                                                1 - последовательность 0 и 1, с переходом на новую строку
    */
    int loadImgFromFile(const char* fileName, int format = 1);
    int saveImgToFile(const char* fileName, int format = 1);
};
