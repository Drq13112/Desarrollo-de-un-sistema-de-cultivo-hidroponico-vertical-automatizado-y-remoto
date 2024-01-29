<a name="readme-top"></a>

[![License](https://img.shields.io/badge/License-BSD_3--Clause-blue.svg)](https://opensource.org/licenses/BSD-3-Clause)
<br />
<div align="center">
  
  <h3 align="center">Development of an automated vertical and remote hydroponic automated and remote vertical</h3>

  <p align="center">
    Development of an automated vertical and remote hydroponic automated and remote vertical
    <br />
    <a href="https://github.com/albertoibernon/Autonomous_Surveillance_Robot/blob/main/project_report/Guiado_y_Navegacion.pdf"><strong>Explore the docs »</strong></a>
    <br />
    <br />
    <a href="#usage">View Demo</a>
    ·
    <a href="#contact">Contact</a>
  </p>
</div>



<!-- TABLE OF CONTENTS -->
<details>
  <summary>Table of Contents</summary>
  <ol>
    <li>
      <a href="#about-the-project">About The Project</a>
      <ul>
        <li><a href="#project-context-and-motivation">Project Context and Motivation</a></li>
        <li><a href="#objectives-of-the-project">Objectives of the Project</a></li>
        <li><a href="#software-architecture">Software Architecture</a></li>
      </ul>
    </li>
    <li><a href="#usage">Usage</a></li>
    <li><a href="#license">License</a></li>
  </ol>
</details>



<!-- ABOUT THE PROJECT -->
## About The Project

In this study, a functional prototype of an automated hydroponic garden capable of working outdoors is developed and built. 
In addition, a control dashboard has been designed and coded that allows the user to monitor and control the garden remotely, and to be notified by alarms about the need of a physical intervention.
The prototype is equipped with energy autonomy through solar panels and batteries.
Likewise, it is built with reusable materials to the maximum extent possible and  optimization of the materials acquired.

### Project Context and Motivation

With each passing year, food and water shortages become more apparent. According to the Food and Agriculture Organisation of the United Nations (FAO), hunger, food shortages and malnutrition affect every country in the world. Moreover, the world wastes 17% of its food, while 811 million people suffer from hunger. Not only that, but it is estimated that the world's population will reach 9.7 billion people by 2050, which will mean a 70% increase in demand for food. 

Faced with this alarming projection, action is urgently needed to ensure food security in the future. To provide solutions to this problem, an automated hydroponic garden has been devised. fully automated, solar-powered, with a high level of productivity and designed to work both outdoors and indoors. and designed to work both outdoors and indoors. This project not only aims to address food challenges, but also to contribute to the Sustainable Development Goals (SDGs) of the 2030 Agenda.

The prototype approach is in line with SDG 11 and SDG 2: Sustainable cities and communities and world hunger zero. sustainable cities and communities and world hunger 0.

The automation and energy self-sufficiency of the prototype allows food to be produced locally, thus reducing dependence on long and costly journeys. 
food locally, thus reducing dependence on long and costly journeys. 
travel. In addition, our commitment to sustainability is reflected in the use of reusable materials and the use of reusable materials and the optimisation of purchased resources. 

Studies Studies by experts in sustainable agriculture show that hydroponic systems can hydroponic systems can significantly increase crop productivity compared to traditional cultivation methods. significantly increase crop productivity compared to traditional cultivation methods (López Elías, 2018).

On the other hand, the large-scale implementation of the developed system is highly The large-scale implementation of the developed system is highly promising, as the design based on independent modules facilitates its scalability, in addition to the benefits it offers, such as efficiency in the use of water and resources. 

This defines it as an energetically sustainable and environmentally friendly agricultural system. environmentally friendly.
In short, our automated hydroponic garden project is more than just a solution to cope with the lack of water and resources. simply a solution to cope with the lack of food.


![Apollo simulations](./figs/marvin_and_laser.png)
*Figure 1: Apollo simulations.*

This selection and the assumptions about the application have conditioned the map (the robot's navigation environment). As can be seen in Figure 2, it presents areas with narrow corridors, small rooms, but also wide spaces and oblique walls. In this way, we have tried to generalize the possibilities to be found in a real museum.

![Visualization of the map in Apolo. (a) Top view. (b) Perspective view.](figs/map.png)
*Figure 2: Visualization of the map in Apolo.*

### Objectives of the Project

Once the application/problem requiring the navigation of a terrestrial robot has been chosen and the map of the environment to be worked on has been defined, we can establish the following objectives for the present work (see *project_report* for all the following chapters):

- By using a differential type locomotion system: determine the **uncertainty associated with the kinematic model** (Chapter 2).
- Chosen the **perception system**: **determine its uncertainty** by means of calibration (Chapter 2).
- For the **localization**: implement an algorithm that allows to obtain at any time the absolute position of the robot **using the Extended Kalman Filter**. Test and experiment with this algorithm to **analyze the influence of the parameters** and the goodness of the estimation achieved (Chapter 3).
- For the movement: **program a controller** that allows the robot to follow a defined trajectory. **Include a reactive control** that allows to avoid obstacles or to navigate through narrow places (Chapter 4).
- For trajectory generation: **integrate a planner** (Chapter 4).
- **Encapsulate everything** as a navigation algorithm as a proposal to the proposed application (Chapter 5).

### Software Architecture

The Robotic Navigation and Control has been implemented following a modular methodology that allows to perform individual regression tests to ease the validation process. It permits to distinguish the several parts involved in the project. It is structured in the following modules:

- **Init.m:** responsible for reading the initial and goal poses, inputs of the simulator. Then, the module places the robot real pose in the initial pose and resets the odometry. It also defines the sensors noise and estimation covariances. Finally, it initialize the time variables and the store matrices.
- **Planificador-global.m:** once the initial and goal poses are identified, the path planning module is executed to generate the waypoints that connect both points, based on RRT-star. This planification process is done off-line, previous to the running of the pseudo real-time (in-the-loop) code. The output of module is a matrix with the waypoints coordinates.
- **Planificador-local.m:** responsible to generate the intermediate reference points that the robot needs to chase between two waypoints during the in-the-loop execution. It permits path re-planning from the last estimated position to the next waypoint once the previous sequence of reference points has been completed, which enhances the success of the path chaser task. The module also extracts the derived forward and rotation velocities commands, computed when the reference points are correlated with the time. These commands are considered as an active control.
- **Navegacion.m:** responsible for taking the information from the proprioceptive (odometry) and exteroceptive (landmarks) sensors which are integrated in the Expanded Kalman Filter (EKF) to obtain the estimation pose and covariance matrices.
- **Control-reactivo.m:** responsible for taking the reference pose and the estimated pose, computing the error between them and extract reactive velocity commands, from a PI controller, which are added to the active commands. It also identifies the present of a close wall or obstacle from ultrasonic sensors and generates a robot rotation, proportional to the distance to the objects, to move away from them. Once, the objects are not detected, the robot performs a replanning, in case of needing.
- **Dke.m:** the simulator takes the control velocity commands and executes them in the Dynamics and Kinematic Environment (DKE), although only kinematic effects are considered for this approach. In this way, the real position and orientation are obtained.

![Robotic Navigation and Control software architecture](figs/architecture.png)
*Figure 3: Robotic Navigation and Control software architecture.*

## Usage
The following is a short video demonstrating the operation of the developed project.
![The gif](./figs/demo.gif)

<p align="right">(<a href="#readme-top">back to top</a>)</p>

<!-- LICENSE -->
## License

Distributed under the BSD 3-Clause License. See `LICENSE.txt` for more information.

<p align="right">(<a href="#readme-top">back to top</a>)</p>


<!-- CONTACT -->
## Contact
Alberto Ibernón Jiménez - [albertoibernon](https://github.com/jbarciv) - alberto.ibernon@alumnos.upm.es\
David Redondo Quintero - [Drq13112](https://github.com/Drq13112) - david.redondo@alumnos.upm.es\
Josep Mª Barberá Civera - [jbarciv](https://github.com/jbarciv) - chemabc@gmail.com

<p align="right">(<a href="#readme-top">back to top</a>)</p>
