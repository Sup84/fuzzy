#include<iostream>
#include<vector>

using namespace std;

enum ServiceQuality {schlecht, gut, hervorragend};
enum EssensQuality {wenig, viel};
enum Operator {AND, OR};
enum Trinkgeld {kaum, normal, grosszuegig};
const vector<string> trliste{"kaum", "normal", "großzügig"}; 

class Maximum {
  friend class FuzzySet;
  double maximum;
  Trinkgeld type;
  Maximum() : maximum{0} {}
};

class FuzzySet {
  double serviceSchlecht;
  double serviceGut;
  double serviceHervorragend;

  double essenWenig;
  double essenViel;

  Maximum maxValue;
  double x1;                                      //For x coordinates of the maximum value
  double x2;
  double x;

  double essenSwitch(EssensQuality es) {
    switch (es) {
      case 0:
        return essenWenig;
      case 1:
        return essenViel;
    }
  }
  double serviceSwitch(ServiceQuality se) {
    switch (se) {
      case 0:
        return serviceSchlecht;
      case 1:
        return serviceGut;
      case 2:
        return serviceHervorragend;
    }
  }

  double trinkgeldCounter(Trinkgeld tg, double value) {
    if (maxValue.maximum < value) {
      maxValue.maximum = value;
      maxValue.type = tg;
    }
  }

  void trinkgeldSwitch() {
    switch (maxValue.type) {
      case 0:
        x1 = 0;
        x2 = 1 / (0.4 * maxValue.maximum) + 2.5;
        break;
      case 1:
        x1 = maxValue.maximum/0.4 + 2.5;
        x2 = 1 / (0.4 * maxValue.maximum) + 5;
        break;
      case 2:
        x1 = maxValue.maximum/0.4 + 5;
        x2 = 10;
        break;        
    }
  }

  double meanOfMaximum() {
    return (x1 + x2) / 2;
  }

public:
  FuzzySet(double service, double essen) {
    if (service > 10 || essen > 10 || service < 0 || essen < 0) throw runtime_error("These parameters do not fit into the fuzzy set.");
    if (service <= 2.5) {                                            //Defining service variables
      serviceSchlecht = 1;
      serviceGut = 0;
      serviceHervorragend = 0;
    }
    if (service > 2.5 && service <= 5) {
      serviceSchlecht = 1 - ((service - 2.5) * 0.4);
      serviceGut = (service - 2.5) * 0.4;
      serviceHervorragend = 0;
    }
    if (service > 5 && service <= 7.5) {
      serviceSchlecht = 0;
      serviceGut = 1 - ((service - 5) * 0.4);
      serviceHervorragend = (service - 5) * 0.4;
    }
    if (service > 7.5) {
      serviceSchlecht = 0;
      serviceGut = 0; 
      serviceHervorragend = 1;
    }
    essenWenig = 1 - (essen * 0.1);                                 //Defining essen variables
    essenViel = essen * 0.1;
  }

  void run() {
    trinkgeldSwitch();
    x = meanOfMaximum();
  }

  double trinkgeldRule(Trinkgeld tg, ServiceQuality se, Operator op, EssensQuality es) {   //Processing rules
    double result;
    if (op == AND) {
      if (essenSwitch(es) < serviceSwitch(se)) result = essenSwitch(es);
      if (essenSwitch(es) >= serviceSwitch(se)) result = serviceSwitch(se);
    }
    if (op == OR) {
      if (essenSwitch(es) < serviceSwitch(se)) result = serviceSwitch(se);
      if (essenSwitch(es) >= serviceSwitch(se)) result = essenSwitch(es);
    }
    trinkgeldCounter(tg, result);
    return result;
  }
  double trinkgeldRule(Trinkgeld tg, ServiceQuality se) {
    double result{serviceSwitch(se)};
    trinkgeldCounter(tg, result);
    return result;
  }
  double trinkgeldRule(Trinkgeld tg, EssensQuality es) {
    double result {essenSwitch(es)};
    trinkgeldCounter(tg, result);
    return result;
  }

  void printData() {
    cout << "serviceSchlecht: " << serviceSchlecht << '\n';
    cout << "serviceGut: " << serviceGut << '\n';
    cout << "serviceHervorragend: " << serviceHervorragend << '\n';
    cout << "essenWenig: " << essenWenig << '\n';
    cout << "essenViel: " << essenViel << '\n';
  }

  ostream &printResult(ostream &o) {
    run();
    return o << "Truthvalue: " << maxValue.maximum << "\nx coordinates: " <<  x1 << ", " << x2 << "\nMean of the coordinates: " << x << '\n' << "Die Mühe der Arbeiter wird " << trliste.at(static_cast<size_t>(maxValue.type)) << " belohnt." << '\n';
  } 
};

ostream &operator<<(ostream &o, FuzzySet &a) {
  a.printResult(o);
  return o;
}

int main() {
  cout << "Service quality? ";
  double s;
  cin >> s;
  cout << "Food quantity? ";
  double f;
  cin >> f;
  FuzzySet a(s,f);

  a.printData();
  a.trinkgeldRule(kaum, schlecht, AND, wenig);
  a.trinkgeldRule(normal, gut);
  a.trinkgeldRule(grosszuegig, hervorragend, OR, viel);
  cout << a;
}
