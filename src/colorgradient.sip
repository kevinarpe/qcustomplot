
%ModuleCode
#include "qcustomplot.h"
%End

class QCPColorGradient
{
%TypeHeaderCode
#include "qcustomplot.h"
%End

public:
  enum ColorInterpolation { ciRGB  ///< Color channels red, green and blue are linearly interpolated
                            ,ciHSV ///< Color channels hue, saturation and value are linearly interpolated (The hue is interpolated over the shortest angle distance)
                          };
//  Q_ENUMS(ColorInterpolation)
  
  enum GradientPreset { gpGrayscale  ///< Continuous lightness from black to white (suited for non-biased data representation)
                        ,gpHot       ///< Continuous lightness from black over firey colors to white (suited for non-biased data representation)
                        ,gpCold      ///< Continuous lightness from black over icey colors to white (suited for non-biased data representation)
                        ,gpNight     ///< Continuous lightness from black over weak blueish colors to white (suited for non-biased data representation)
                        ,gpCandy     ///< Blue over pink to white
                        ,gpGeography ///< Colors suitable to represent different elevations on geographical maps
                        ,gpIon       ///< Half hue spectrum from black over purple to blue and finally green (creates banding illusion but allows more precise magnitude estimates)
                        ,gpThermal   ///< Colors suitable for thermal imaging, ranging from dark blue over purple to orange, yellow and white
                        ,gpPolar     ///< Colors suitable to emphasize polarity around the center, with blue for negative, black in the middle and red for positive values
                        ,gpSpectrum  ///< An approximation of the visible light spectrum (creates banding illusion but allows more precise magnitude estimates)
                        ,gpJet       ///< Hue variation similar to a spectrum, often used in numerical visualization (creates banding illusion but allows more precise magnitude estimates)
                        ,gpHues      ///< Full hue cycle, with highest and lowest color red (suitable for periodic data, such as angles and phases, see \ref setPeriodic)
                      };
//  Q_ENUMS(GradientPreset)
  
  QCPColorGradient(GradientPreset preset=gpCold);
  bool operator==(const QCPColorGradient &other) const;
  bool operator!=(const QCPColorGradient &other) const;
  
  // getters:
  int levelCount() const;
// TODO: unsupported function return type - provide %MethodCode and a C++ signature
//  QMap<double, QColor> colorStops() const;
  ColorInterpolation colorInterpolation() const;
  bool periodic() const;
  
  // setters:
  void setLevelCount(int n);
// TODO: unsupported function argument type - provide %MethodCode and a C++ signature
//  void setColorStops(const QMap<double, QColor> &colorStops);
  void setColorStopAt(double position, const QColor &color);
  void setColorInterpolation(ColorInterpolation interpolation);
  void setPeriodic(bool enabled);
  
  // non-property methods:
  void colorize(const double *data, const QCPRange &range, QRgb *scanLine, int n, int dataIndexFactor=1, bool logarithmic=false);
  QRgb color(double position, const QCPRange &range, bool logarithmic=false);
  void loadPreset(GradientPreset preset);
  void clearColorStops();
  QCPColorGradient inverted() const;
  
protected:
  void updateColorBuffer();
  
  // property members:
//  int mLevelCount;
//  QMap<double, QColor> mColorStops;
//  ColorInterpolation mColorInterpolation;
//  bool mPeriodic;
  
  // non-property members:
//  QVector<QRgb> mColorBuffer;
//  bool mColorBufferInvalidated;
};

