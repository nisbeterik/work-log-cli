# work-log-cli
CLI application written in C to log time worked and generate reports of months/years.

### Requirements

Compiling the C File
With GCC (Linux/Unix-based systems)
To compile the C file using GCC, use the following command in your terminal:

``gcc -o work_logger work_logger.c``

This will compile the work_logger.c source code into an executable called work_logger. You can then run it by simply typing:


``./work_logger``

With Visual Studio Compiler (Windows)
To compile the C file on Windows using the Visual Studio compiler (MSVC), follow these steps:

Open the Developer Command Prompt for Visual Studio.
Navigate to the directory where work_logger.c is located.
Run the following command to compile the C file:

``cl work_logger.c``
This will generate an executable named work_logger.exe. You can then run it with:

``work_logger.exe``
Running the Application
Running the Compiled File
Once the program has been compiled (using either GCC or MSVC), you can run the generated executable by simply typing:

``./work_logger``
on Linux/macOS, or:

``work_logger.exe``
on Windows. This will start the application and allow you to interact with it.

## Piping Minutes as an Argument
You can also run the program and directly provide the number of minutes worked by piping the value as an argument. For example:

``echo 120 | ./work_logger``
This will pass 120 as the number of minutes worked to the program. It will log this time as part of your data.

Alternatively, you can use the following syntax:

``./work_logger 120``
This will directly pass 120 as a command-line argument to the program.
