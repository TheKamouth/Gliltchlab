#ifndef GLILCTHLABCORE_H
#define GLILCTHLABCORE_H

#include <QImage>
#include <QFileDialog>
#include <QMovie>

#include <map>
#include <iostream>
#include <math.h>
#include <vector>
#include <algorithm>

//#include "lib/gif.h"

#include "glitchparameters.h"
#include "glitchsortingrule.h"
#include "glitchmask.h"

#define GliltchLabCoreTools Symmetry

#define ToolsFunctions(tool) void Process();

class GlilcthLabCore: public QObject
{
    Q_OBJECT

    struct myComparator {
      bool operator() (QRgb rgb0, QRgb rgb1) {

          if( rule == 0){ return false;}

          int v0 = rule->GetColorValue( QColor(rgb0));
          int v1 = rule->GetColorValue( QColor(rgb1));

          return isInverted ? (v1 < v0 ):(v0 < v1 );
      }
      GlitchSortingRule *rule;
      bool isInverted;
    } myCompClass;

public:
    GlilcthLabCore();

    void SetInputImage(QImage * image);
    void SetInputPixmap(QPixmap & pixmap);

    void ProcessGlitchRefactor();
    void ProcessSymmetryStart();
    void LoadGlitchParameters();
    void SaveGlitchParameters();

    QSize GetInputImageSize();
    QPixmap * GetInputImage();
    QImage * GetOuputImage(){ return outputQImage;}

    void SetSortingAngle( float angle);
    void SetCircularSorting( bool);
    void SetCircularSortingCenter( QPointF);

    void SetColorToValueFunction( SortingRule sortingRule);
    void InvertSortingRule( bool);
    void SetMask( QImage*, MaskType);
    void SetSegmentationRule(bool isSegmentationEnabled, int threshold);
    QImage * DeformWith2DFunction();
    void AbortProcess();
    void ClearResources();

public slots:
    void ProcessGlitch();

signals:
   void ShowOutput( QImage* output);
   void SendProcessGlitchDone();
   void SendProcessSymmetryDone();
   void SendProcessProgress( int);

private:

   void Init();

    GlitchParameters* glitchParameters;

    QPixmap * inputImage;

    QImage * outputQImage;

    bool processGlitchOnChange;

    bool abortProcess;

    bool areResourcesAllocated;

    //IMAGE PROCESSING FUNCTION
    void AddPixelToMap(std::map< int, QRgb> &map, int pixelIndex, QRgb color);
    void DrawLine(std::vector< QRgb> *line, QPoint p0, QPoint p1);
    void DrawLine(std::vector< QRgb>::reverse_iterator lineIterator, QPoint p0, QPoint p1);

    //new working:
    void Bresenham(QPoint _p0, QPoint _p1, std::vector< QRgb> *line, void (*function)(int, int, std::vector<QRgb>*, QPixmap*, QImage*, int));
    int GetOctantFromAngle(float angle);
    QPoint SwitchToOctanZeroFrom( int octant, int x, int y);
    QPoint SwitchFromOctanZeroTo( int octant, int x, int y);
    bool isInImageRange( QPoint p);

    //void GetPixel(int x, int y, std::vector< QRgb>*);

    void (*getPixel)(int, int, std::vector<QRgb>*, QPixmap*, QImage*, int);
    void (*drawPixel)(int, int, std::vector<QRgb>*, QPixmap*, QImage*, int);

    //GIF FUNCTION
    void MakeRotatingGIF();

    //TESTS
    void DrawTestLine();

    //
    bool ComparisonFunction ( QRgb i, QRgb j);
};

#endif // GLILCTHLABCORE_H
