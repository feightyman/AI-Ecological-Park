#ifndef APPLICATIONS_BMP280_H_
#define APPLICATIONS_BMP280_H_

//气压传感器
void BMP280_Start();
void BMP280_Run();

extern float BMP_Temperature;
extern float BMP_Pressure;
extern float BMP_Approx;

#endif /* APPLICATIONS_BMP280_H_ */
