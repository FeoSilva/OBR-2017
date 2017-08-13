/*
  QTRSensors.h - Library for using Pololu QTR reflectance
    sensors and reflectance sensor arrays: QTR-1A, QTR-8A, QTR-1RC, and
    QTR-8RC.  The object used will determine the type of the sensor (either
    QTR-xA or QTR-xRC).  Then simply specify in the constructor which
    Arduino I/O pins are connected to a QTR sensor, and the read() method
    will obtain reflectance measurements for those sensors.  Smaller sensor
    values correspond to higher reflectance (e.g. white) while larger
    sensor values correspond to lower reflectance (e.g. black or a void).

    * QTRSensorsRC should be used for QTR-1RC and QTR-8RC sensors.
    * QTRSensorsAnalog should be used for QTR-1A and QTR-8A sensors.
*/

/*
 * Written by Ben Schmidel et al., October 4, 2010
 * Copyright (c) 2008-2012 Pololu Corporation. For more information, see
 *
 *   http://www.pololu.com
 *   http://forum.pololu.com
 *   http://www.pololu.com/docs/0J19
 *
 * You may freely modify and share this code, as long as you keep this
 * notice intact (including the two links above).  Licensed under the
 * Creative Commons BY-SA 3.0 license:
 *
 *   http://creativecommons.org/licenses/by-sa/3.0/
 *
 * Disclaimer: To the extent permitted by law, Pololu provides this work
 * without any warranty.  It might be defective, in which case you agree
 * to be responsible for all resulting costs and damages.
 */

#ifndef QTRSensors_h
#define QTRSensors_h

#define QTR_EMITTERS_OFF 0
#define QTR_EMITTERS_ON 1
#define QTR_EMITTERS_ON_AND_OFF 2

#define QTR_NO_EMITTER_PIN  255

#define QTR_MAX_SENSORS 16

// This class cannot be instantiated directly (it has no constructor).
// Instead, you should instantiate one of its two derived classes (either the
// QTR-A or QTR-RC version, depending on the type of your sensor).
class QTRSensors
{
  public:

    // Reads the sensor values into an array. There *MUST* be space
    // for as many values as there were sensors specified in the constructor.
    // Example usage:
    // unsigned int sensor_values[8];
    // sensors.read(sensor_values);
    // The values returned are a measure of the reflectance in abstract units,
    // with higher values corresponding to lower reflectance (e.g. a black
    // surface or a void).
    // If measureOffAndOn is true, measures the values with the
    // emitters on AND off and returns on - (timeout - off).  If this
    // value is less than zero, it returns zero.
    // This method will call the appropriate derived class's readPrivate(),
    // which is defined as a virtual function in the base class and
    // overridden by each derived class's own implementation.
    void read(unsigned int *sensor_values, unsigned char readMode = QTR_EMITTERS_ON);

    // Turn the IR LEDs off and on.  This is mainly for use by the
    // read method, and calling these functions before or
    // after the reading the sensors will have no effect on the
    // readings, but you may wish to use these for testing purposes.
    void emittersOff();
    void emittersOn();

    // Reads the sensors for calibration.  The sensor values are
    // not returned; instead, the maximum and minimum values found
    // over time are stored internally and used for the
    // readCalibrated() method.
    void calibrate(unsigned char readMode = QTR_EMITTERS_ON);

    // Resets all calibration that has been done.
    void resetCalibration();

    // Returns values calibrated to a value between 0 and 1000, where
    // 0 corresponds to the minimum value read by calibrate() and 1000
    // corresponds to the maximum value.  Calibration values are
    // stored separately for each sensor, so that differences in the
    // sensors are accounted for automatically.
    void readCalibrated(unsigned int *sensor_values, unsigned char readMode = QTR_EMITTERS_ON);

    // Operates the same as read calibrated, but also returns an
    // estimated position of the robot with respect to a line. The
    // estimate is made using a weighted average of the sensor indices
    // multiplied by 1000, so that a return value of 0 indicates that
    // the line is directly below sensor 0, a return value of 1000
    // indicates that the line is directly below sensor 1, 2000
    // indicates that it's below sensor 2000, etc.  Intermediate
    // values indicate that the line is between two sensors.  The
    // formula is:
    //
    //    0*value0 + 1000*value1 + 2000*value2 + ...
    //   --------------------------------------------
    //         value0  +  value1  +  value2 + ...
    //
    // By default, this function assumes a dark line (high values)
    // surrounded by white (low values).  If your line is light on
    // black, set the optional second argument white_line to true.  In
    // this case, each sensor value will be replaced by (1000-value)
    // before the averaging.
    int readLine(unsigned int *sensor_values, unsigned char readMode = QTR_EMITTERS_ON, unsigned char white_line = 0);

    // Calibrated minumum and maximum values. These start at 1000 and
    // 0, respectively, so that the very first sensor reading will
    // update both of them.
    //
    // The pointers are unallocated until calibrate() is called, and
    // then allocated to exactly the size required.  Depending on the
    // readMode argument to calibrate, only the On or Off values may
    // be allocated, as required.
    //
    // These variables are made public so that you can use them for
    // your own calculations and do things like saving the values to
    // EEPROM, performing sanity checking, etc.
    unsigned int *calibratedMinimumOn;
    unsigned int *calibratedMaximumOn;
    unsigned int *calibratedMinimumOff;
    unsigned int *calibratedMaximumOff;

    ~QTRSensors();

  protected:

    QTRSensors()
    {

    };

    void init(unsigned char *pins, unsigned char numSensors, unsigned char emitterPin);

    unsigned char *_pins;
    unsigned char _numSensors;
    unsigned char _emitterPin;
    unsigned int _maxValue; // the maximum value returned by this function
    int _lastValue;

  private:

    virtual void readPrivate(unsigned int *sensor_values) = 0;

    // Handles the actual calibration. calibratedMinimum and
    // calibratedMaximum are pointers to the requested calibration
    // arrays, which will be allocated if necessary.
    void calibrateOnOrOff(unsigned int **calibratedMinimum,
                          unsigned int **calibratedMaximum,
                          unsigned char readMode);
};



// Object to be used for QTR-1RC and QTR-8RC sensors
class QTRSensorsRC : public QTRSensors
{
  public:

    // if this constructor is used, the user must call init() before using
    // the methods in this class
    QTRSensorsRC();

    // this constructor just calls init()
    QTRSensorsRC(unsigned char* pins, unsigned char numSensors,
          unsigned int timeout = 4000, unsigned char emitterPin = 255);

    // The array 'pins' contains the Arduino pin number for each sensor.

    // 'numSensors' specifies the length of the 'pins' array (i.e. the
    // number of QTR-RC sensors you are using).  numSensors must be
    // no greater than 16.

    // 'timeout' specifies the length of time in microseconds beyond
    // which you consider the sensor reading completely black.  That is to say,
    // if the pulse length for a pin exceeds 'timeout', pulse timing will stop
    // and the reading for that pin will be considered full black.
    // It is recommended that you set timeout to be between 1000 and
    // 3000 us, depending on things like the height of your sensors and
    // ambient lighting.  Using timeout allows you to shorten the
    // duration of a sensor-reading cycle while still maintaining
    // useful analog measurements of reflectance

    // 'emitterPin' is the Arduino pin that controls the IR LEDs on the 8RC
    // modules.  If you are using a 1RC (i.e. if there is no emitter pin),
    // or if you just want the emitters on all the time and don't want to
    // use an I/O pin to control it, use a value of 255 (QTR_NO_EMITTER_PIN).
    void init(unsigned char* pins, unsigned char numSensors,
          unsigned int timeout = 2000, unsigned char emitterPin = QTR_NO_EMITTER_PIN);



  private:

    // Reads the sensor values into an array. There *MUST* be space
    // for as many values as there were sensors specified in the constructor.
    // Example usage:
    // unsigned int sensor_values[8];
    // sensors.read(sensor_values);
    // The values returned are a measure of the reflectance in microseconds.
    void readPrivate(unsigned int *sensor_values);
};



// Object to be used for QTR-1A and QTR-8A sensors
class QTRSensorsAnalog : public QTRSensors
{
  public:

    // if this constructor is used, the user must call init() before using
    // the methods in this class
    QTRSensorsAnalog();

    // this constructor just calls init()
    QTRSensorsAnalog(unsigned char* pins,
        unsigned char numSensors, unsigned char numSamplesPerSensor = 4,
        unsigned char emitterPin = 255);

    // the array 'pins' contains the Arduino analog pin assignment for each
    // sensor.  For example, if pins is {0, 1, 7}, sensor 1 is on
    // Arduino analog input 0, sensor 2 is on Arduino analog input 1,
    // and sensor 3 is on Arduino analog input 7.

    // 'numSensors' specifies the length of the 'analogPins' array (i.e. the
    // number of QTR-A sensors you are using).  numSensors must be
    // no greater than 16.

    // 'numSamplesPerSensor' indicates the number of 10-bit analog samples
    // to average per channel (i.e. per sensor) for each reading.  The total
    // number of analog-to-digital conversions performed will be equal to
    // numSensors*numSamplesPerSensor.  Note that it takes about 100 us to
    // perform a single analog-to-digital conversion, so:
    // if numSamplesPerSensor is 4 and numSensors is 6, it will take
    // 4 * 6 * 100 us = ~2.5 ms to perform a full readLine().
    // Increasing this parameter increases noise suppression at the cost of
    // sample rate.  The recommended value is 4.

    // 'emitterPin' is the Arduino pin that controls the IR LEDs on the 8RC
    // modules.  If you are using a 1RC (i.e. if there is no emitter pin),
    // or if you just want the emitters on all the time and don't want to
    // use an I/O pin to control it, use a value of 255 (QTR_NO_EMITTER_PIN).
    void init(unsigned char* analogPins, unsigned char numSensors,
        unsigned char numSamplesPerSensor = 4, unsigned char emitterPin = QTR_NO_EMITTER_PIN);



  private:

    // Reads the sensor values into an array. There *MUST* be space
    // for as many values as there were sensors specified in the constructor.
    // Example usage:
    // unsigned int sensor_values[8];
    // sensors.read(sensor_values);
    // The values returned are a measure of the reflectance in terms of a
    // 10-bit ADC average with higher values corresponding to lower
    // reflectance (e.g. a black surface or a void).
    void readPrivate(unsigned int *sensor_values);

    unsigned char _numSamplesPerSensor;
};


#endif
