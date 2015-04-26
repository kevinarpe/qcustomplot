
%ModuleCode
#include "qcustomplot.h"
%End

class QCPRange
{
%TypeHeaderCode
#include "qcustomplot.h"
%End

public:
//  double lower, upper;
  double lower;
  double upper;
  
  QCPRange();
  QCPRange(double lower, double upper);
  
  bool operator==(const QCPRange& other) const;
  bool operator!=(const QCPRange& other) const;
  
  QCPRange &operator+=(const double& value);
  QCPRange &operator-=(const double& value);
  QCPRange &operator*=(const double& value);
  QCPRange &operator/=(const double& value);
//  friend inline const QCPRange operator+(const QCPRange&, double);
//  friend inline const QCPRange operator+(double, const QCPRange&);
//  friend inline const QCPRange operator-(const QCPRange& range, double value);
//  friend inline const QCPRange operator*(const QCPRange& range, double value);
//  friend inline const QCPRange operator*(double value, const QCPRange& range);
//  friend inline const QCPRange operator/(const QCPRange& range, double value);
  
  double size() const;
  double center() const;
  void normalize();
  void expand(const QCPRange &otherRange);
  QCPRange expanded(const QCPRange &otherRange) const;
  QCPRange sanitizedForLogScale() const;
  QCPRange sanitizedForLinScale() const;
  bool contains(double value) const;
  
  static bool validRange(double lower, double upper);
  static bool validRange(const QCPRange &range);
  static const double minRange; //1e-280;
  static const double maxRange; //1e280;
  
};

const QCPRange operator+(const QCPRange& range, double value);
const QCPRange operator+(double value, const QCPRange& range);
const QCPRange operator-(const QCPRange& range, double value);
const QCPRange operator*(const QCPRange& range, double value);
const QCPRange operator*(double value, const QCPRange& range);
const QCPRange operator/(const QCPRange& range, double value);

