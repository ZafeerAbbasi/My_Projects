## Quarter Car Suspension System Modelling

### Project Description
In this project the quarter-car suspension model is used to study the dynamics of a vehicle suspension. The quarter car model consists of the wheel, 
quarter of the vehicle's mass and suspension components such as dampers and springs. The quarter car model will use lumped parameters, which means that 
parameters such as mass, spring stiffness, and damping coefficient will be treated as lumped parameters to simplify the system. This further means the 
system can be studied with ordinary differential equations, since the dependent variables will be functions of time.

First a mathematical model and transfer function is derived for the quarter-car suspension model. The system is then represented in state-space form to 
study the behavior of the state variables. Then an analogous system is derived for the translational mechanical quarter-car suspension model. The system 
is also represented as a linear and bond graph. The respective graphical representation is then accompanied by corresponding equations. Finally the system 
is simulated using Matlab and Simulink to gain a better understanding of its behavior. In our project the output is either the displacement of the wheel 
and/or the displacement of the vehicle body. With the input being the road profile, thus by altering the road profile when adding sudden peaks and troughs 
we can test the behavior of the suspension system. The goal will be to maximize the comfort of the vehicle occupants by decreasing the speed at which the 
body of the car moves when going over bumps in the road. The effects of altering damping, spring and unsprung mass values of the car will be studied in 
detail using simulation data from MATLAB. The objective of the project is to study how changing parameters affect the ride quality of the vehicle.


### Quarter Car Suspension Model

![myimage](https://user-images.githubusercontent.com/86879362/212531546-ec31c5e7-8900-46ed-b235-8857a270973c.jpg)

### Variable Definition Table
Variable Name | Unit          | Description
------------- | ------------- | -----------
y<sub>1</sub>(t) | m  | Displacement of Vehicle Body (output)
y<sub>2</sub>(t) | m  | Displacement of Vehicle Wheel (output)
u(t) | m  | Road Profile (input)
M<sub>1</sub>  | Kg  | Mass of Quarter Vehicle
M<sub>2</sub>  | Kg  | Mass of Wheel and Suspension
K<sub>1</sub>  | N/m | Spring Constant of Suspension System
K<sub>2</sub>  | N/m | Spring Constant of Wheel and Suspension
B<sub>1</sub>  | N * s/m | Damping Constant of Suspension System
B<sub>2</sub>  | N * s/m | Damping Constant of Wheel and Tire


### Some of the results
![RealisticRoadProfileResponseUsingFourierSeries_noderivative](https://user-images.githubusercontent.com/86879362/212531747-300f7273-9cc1-47ba-8f45-3477dfe5741c.jpg)

![RealisticRoadProfileResponseVaryingDamping_noderivative](https://user-images.githubusercontent.com/86879362/212531688-ee548139-ac7e-408a-b243-cb2fd67414f0.jpg)

![RealisticRoadProfileResponseVaryingVehicleWeight_noderivative](https://user-images.githubusercontent.com/86879362/212531691-2d67c9c5-69a0-46dc-8e62-dc6bf6313380.jpg)

