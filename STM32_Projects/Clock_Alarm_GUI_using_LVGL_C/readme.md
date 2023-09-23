# Clock Alarm GUI using LVGL on STM32F429I-DISCOVERY 

## Project Description
This Project intents to create an Alarm Clock GUI Application using an STM32 MCU, specifically, the STM32F429I-Discovery Board. 

- Platform: PlatformIO on VSCode
- Hardware: STM32F429I-Discovery Board

## Demonstration

### Simulation
Utilizing the LVGL simulator for development provided a streamlined and efficient approach to GUI design. The simulator allowed for rapid prototyping, immediate visual feedback, and the ability to troubleshoot and refine the interface without the constraints or potential risks associated with physical hardware. This method ensured a robust and well-tested GUI foundation. Once this foundational development was complete, transitioning to the physical board was the logical next step. On the board, I focused on making the final, minor adjustments, ensuring that the GUI seamlessly integrated with the hardware and fine-tuning any device-specific behaviors or interactions. 

<video src="clockAlarmUISim.mp4" controls title="Title"></video>

### On the Physical Board
I used a stylus because its more precise, but it works completely the same when touching it with your hands.
The video showcasing the use of the GUI on the physical board displayed unusual colors, primarily due to the lighting environment in which the recording took place. Ambient lighting, reflections, and the angle of the camera in relation to the board's screen can all influence the captured colors, potentially leading to discrepancies between the actual display and its representation in the video. Additionally, other factors such as the camera's sensor quality, white balance settings, and post-processing might have contributed to the color anomalies observed in the footage.

<video src="IMG_0044.mp4" controls title="Title"></video>


## How you can run the simulator
Download the repo contents and navigate to /RunSimulatorYourself and simply double click on 'program.exe'. 
- NOTE: Currently only Windows is supported, if enough people ask me, I'll make an executable for MacOS as well.

## Key Features:
1. **Light Mode/Dark Mode**
    -  Users can switch between a light and dark theme using a switch placed on the main screen. Themes are made from color gradients. Font/Border colors also change based on theme: Light: Black Font, Dark: White Font.
2. **Button Animations**
    - Provides a 'Ripple' effect, shadow effect, color gradient, and vertical translation on the 'Save' button when pressed. This serves the following purposes:
        - **Feedback:** Provides immediate feedback to the user, indicating thier action has been recogonized
        - **Depth and Layering:** Shadows and vertical translation give a sense of depth, making the interface feel more three-dimensional and layerd. This makes the UI feel more tactile and intuitive.
        - **Aesthetics and Delight:** The subtle effects make the interface more polished, giving a more aesthetically pleasing interface. This can cause the the user experience to be more enjoyable, which leads to a more positive perception of the application/software.
3. **Auto Save Detection**
    - The GUI automatically detects when the user makes changes but doesnt explicity save them using the 'Save' button. As the user attempts to leave the settings without explicitly saving, the GUI detects this and prompts the user weather they would like to save the changes or not. This is achieved by storing all the users changes before saving. If the user explicitly clicks the save button then the settings are implemented immediately. 
4. **Date Selection**
    - The GUI provides a simple calendar widget with transparent background so the users can easily change the current day, year and month.
5. **Auto Day Increment**
    - Automatically increments day, adjusts for time change and etc. When a new day has occured.
6. **Time Selection with 24Hour Format Option**
    - Provides a simple interface for adjusting time using 3 rollers with transparent backgrounds and a checkbox widget for selecting the meridiem; AM or PM. The Time Page also has a 24 Hour switch that immediately implements changes based on the switch value.

## Current Consumption
The MCU has a power consumption of 70mA. To put that into perspective, if powered by a battery with a capacity of 2250mAh, the MCU would theoretically operate for approximately 32 hours before the battery is depleted. However, it's important to note that this is a simplistic calculation and actual battery life can vary based on numerous factors. Moreover, there were several measures I could have implemented to further optimize power consumption. For instance, when the GUI is in its normal ticking mode, reducing the screen brightness and enabling specific low-power features would have significantly decreased the current draw, thereby extending the battery's operational duration.

### Limitations
1. **Display slider**
    - The Display slider is currently not supported but a future improvement would be to implement the display slider.
    Here's how it would work: 
        - Whenever the value of the slider would change, we would call a ISR. The ISR would save the current value of the slider and call a function to process the change. In the process function we could send a PWM Signal with Duty Cycle equal to the current value of the slider to the brightness pin of the LCD.
2. **Audio**
    - The STM32F429I-Discovery board does not have a audio module. A future improvement can be to attach a Piezo, and send a high frequency PWM Signal to the piezo at the alarm time, causing the peizo to vibrate thus creating a sound effect. 

## What is LVGL?
LVGL, which stands for "Light and Versatile Graphics Library," is an open-source graphics library written in C that provides everything needed to create embedded GUIs (Graphical User Interfaces) with easy-to-use graphical elements, beautiful visual effects, and a low memory footprint.

Here are some key features and aspects of LVGL:

1. **Platform-agnostic**
    - LVGL is designed to be used with a variety of hardware configurations, from high-resolution displays to limited-resource microcontrollers. It doesn't rely on any specific hardware or software platform.

2. **Low Memory Footprint** 
    - It's optimized for embedded systems, which often have limited memory and computational power. As such, LVGL is designed to be efficient and lightweight.

3. **Modular**
    - You can enable or disable parts of the library to save space if certain features or widgets are not needed.

4. **Customizable**  
    - LVGL supports a wide range of widgets with advanced visual effects, and it's also possible to add custom widgets.

5. **Touch Support** 
    - The library supports touchpad-based interactions, making it suitable for modern touch-enabled devices.

6. **Graphics Features**
    - LVGL provides anti-aliasing, opacity, animations, and other graphical features that allow for the creation of visually appealing interfaces.

7. **Active Community** 
    - Being open-source, LVGL has a community of developers and users who contribute to its development, provide support, and share resources.

8. **Licensing**
    - LVGL is licensed under the MIT license, which is permissive and allows for use in both open-source and commercial applications.