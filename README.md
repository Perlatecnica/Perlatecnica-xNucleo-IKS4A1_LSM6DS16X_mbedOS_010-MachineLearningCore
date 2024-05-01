# xNucleo-IKS4A1_LSM6DSV16X_mbedOS_010-MachineLearningCore

This application shows how to detect the activity using the LSM6DSV16X Machine Learning Core.

---

### Description:

This code demonstrates the usage of Machine Learning Core (MLC) within the LSM6DSV16X sensor to perform activity recognition. It loads a pre-defined program for activity recognition into the sensor's MLC and continuously monitors the sensor's output to detect different activities such as stationary, walking, jogging, biking, or driving. Detected activities are printed over the serial connection.

### How it works:

- **Initialization**: 
  - It includes necessary libraries for communication (`mbed.h`), for interfacing with the LSM6DSV16X sensor (`plt_iks4a1.h`), and for the MLC program (`lsm6dsv16x_activity_recognition_for_mobile.h`).
  - It initializes objects for serial communication (`Serial`), an LED for visual indication, and an interrupt pin (`InterruptIn`).
  - It sets up an interrupt callback function (`INT1Event_cb`) to handle interrupts.

- **MLC Program Loading**: 
  - It loads a pre-defined MLC program for activity recognition into the LSM6DSV16X sensor.
  - It feeds the program line by line to the sensor's MLC.

- **Main Loop**:
  - It waits for a time window before checking the MLC status for the first time.
  - It continuously monitors the MLC output and prints the detected activity over the serial connection.

- **Interrupt Event Handler**:
  - When the interrupt occurs, it sets a flag to indicate an event.

- **Print MLC Status Function**:
  - It prints the detected activity based on the MLC output.

---

This description provides an overview of the code's functionality, including MLC program loading, activity recognition, and printing the detected activities.

