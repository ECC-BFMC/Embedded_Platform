@dontinclude main_ex1.cpp
In this second tutorial we explain the usage of the quadratic encoder and the pid controller,by which you can control the robot with a constant forward speed.
If you want to control the forward speed of the robot, than you need an rotary encoder, which can read the rotation speed of the motor. You can create with the next line a filter, which is used to minimize the noise. The input parameters of the filter are two list of number, which are the coefficients of the polynomials. You can rewrite a discrete filter with infinite impulse response in the next form: 
\image html tf_eq.png
<!-- \f$\frac{Y(z^{-1})}{X(z^{-1})}= \frac{b_0+b_1 z^{-1}+ ... + b_n z^{-n}}{1+a_1 z^{-1}+ ... + a_m z^{-m}}\f$ -->
,in this case you can create the filter by the next line of code:

    filter::lti::siso::CIIRFilter<float,m-1,n> g_encoderFilter(linalg::CRowVector<float,1m-1({ a1,....,am}),linalg::CRowVector<float,n>({b0,b1,...,bn}));

In our case the discrete-time filter is described by two second degree polynomials, and is initialized by the next line of code. 
@snippet main_ex1.cpp Create encoder filter
For measuring the rotation speed of the motor, you need an rotary encoder. The rotary encoder generates a quadrature signal, which can give the position, the orientation  and the rotation speed of the motor, this type of encoders convert angular rotation into two pulse signal. You can process the signal by using InterruptIn or hardware Timer to decode the signal. You can create a driver object based on the hardware Timer with the following line, in this case the pins are predefined (PB_6 and PB_7).
@skipline g_quadratureEncoderTask(
This driver based on the Rtos timer, which applies periodically the run method to measure and filtrate the angular speed, the first parameter is the period of the task, the second parameter is the interface to the hardware Timer. You need to set the resolution of the encoder (counts per revolution) and to give the filter object. If you initialized in the right way, than the object periodically measure and filtrate the angular speed. 
You can create an new object to transmit the angular speed over the serial interface to another device. You can use CEncoderSender class, which is a CTask object. You need to set the period, the encoder object and the serial interface:
@skipline g_encoderPublisher(
There are several ways you can initialize a pid controller. If you need a converter function, which transform the control output signal to the process input signal, than you can create one, like in the following line:
@skipline l_volt2pwmConverter(
In this case, the control output signal is the voltage of the dc motor, but the micro-controller can generate a PWM signal to regulate the voltage level, this convert function calculate the pwn signal based on the requested voltage level. If you have the discrete-time transfer function of the pid controller, than you can initialize it:
@skipline g_motorPIDTF(
This object implements the functionality of the discrete-time transfer function, it's necessary to give two list of coefficients, where the first represent the polynomial of the numerator part, the second list symbolize the polynomial of the denominator part. You can create a pid controller object by adding the transfer function and the period, this object apply the transfer function. You need to create CControllerSiso object, which calculate the error and apply the converter, it can be created by adding the encoder object, the pid controller object and the converter object (optional). 
@snippet main_ex1.cpp Create PID controller
If you created the siso controller, you can give it the constructor of motion controller, when you activate the pid control functionality by receive a PIDA command, it will apply the control in each cycle of the motion controller. You can set the reference value by transmit a MTCL command. If you have not calculated the discrete time form if the pid controller, than you can create your pid controller object by the pid parameters (Kp,Kd,Ki,Tf), where Kp, Kd and Ki denote the coefficients for the proportional, integral and derivative terms respectively. The Tf is the time constant of the first-order filter,the constants are all non-negative real numbers. You must define the sample time of the controller. You can see an example line in the next paragraph:
@skipline l_pidController2(

<!-- PID Controller - initialize by parameter -->

Finally the main source file will contain the same code: 
\includelineno main_ex1.cpp