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
    //����� ����� ��������� ������ ���������� � �������
public :
    //����� ������ ��������� ������� � ����������
    /**/
    ImageProcess();

    /* ���������� ������ ��� �������� ������� w*h */
    ImageProcess(int w, int h);

    /* ���������� ������ ��� �������� ������� w*h � ���������� ��������*/
    ImageProcess(const Img* img);

    /* ���������� ������ ��� �������� ���������� �������� �� �����*/
    ImageProcess(const char* fileName);

    ~ImageProcess();

    /*������� �����*/
    int updateMask(const Img& mask);

    /*������������ �������� �������� ���������, ������� ���� �������� � ���������� ��������� �/��� ������*/
    int updateSrcImg();

    /*��������� ��������, ��������� �������� � processedImg*/
    int dilotation();

    /*������ ��������, ��������� �������� � processedImg*/
    int erosion();

    /*
        ���������/��������� �� ����� � ������ fileName ��������
        input		:	fileName - ��� �����
                        format - ������ ����� 	0 - ������������������ 0 � 1, ��� �������� �� ����� ������
                                                1 - ������������������ 0 � 1, � ��������� �� ����� ������
    */
    int loadImgFromFile(const char* fileName, int format = 1);
    int saveImgToFile(const char* fileName, int format = 1);
};
