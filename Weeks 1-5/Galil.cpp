#include "Galil.h"

// Default constructor. Initialize variables, open Galil connection and allocate memory.
// Should assign a default embedded functions that works with physical hardware and a 
// default Galil address as described in the assignment spec.
Galil::Galil() {
    Functions = new EmbeddedFunctions(false);
    ReadBuffer = new char[1024]; // Allocate buffer for reading Galil responses
    ControlParameters[0] = 0; // Initialize control parameters (Kp, Ki, Kd)
    ControlParameters[1] = 0;
    ControlParameters[2] = 0;
    setPoint = 0;
    GResult = Functions->GOpen(this->IPAddress, &g); // Attempt to open a connection with the default IP address
    if (GResult != 0) {
        std::cerr << "Error: Failed to open Galil connection with default IP address." << std::endl;
    }
    else {
        std::cout << "Success! Opened Galil connection with default IP address." << std::endl;
    }
}

// Constructor with EmbeddedFunction pre-initialised and passed in.
Galil::Galil(EmbeddedFunctions* Funcs, GCStringIn address) {
    Functions = Funcs; // Use the provided EmbeddedFunctions object
    ReadBuffer = new char[1024]; // Allocate buffer for reading Galil responses
    GResult = Functions->GOpen(address, &g); // Open a connection with the specified address
    if (GResult != 0) {
        std::cerr << "Error: Failed to open Galil connection with address: " << address << std::endl;
    }
    else {
        std::cout << "Success! Opened Galil connection with address:" << address << std::endl;
    }}

// Copy constructor to copy the state of all elements within the object other.
// It should construct a new EmbeddedFunctions object and open a separate connection
// (i.e., each class will have a unique value of the GCon g). All other data members
// should be transferred.
Galil::Galil(const Galil& other) {
    Functions = new EmbeddedFunctions(false); // Create a new EmbeddedFunctions object
    ReadBuffer = new char[1024]; // Allocate a new buffer for the new connection
    GResult = Functions->GOpen(IPAddress, &g); // Open a new connection for the copied object
    // Copy control parameters ans setpoint
    ControlParameters[0] = other.ControlParameters[0];
    ControlParameters[1] = other.ControlParameters[1];
    ControlParameters[2] = other.ControlParameters[2];
    setPoint = other.setPoint;
    if (GResult != 0) {
        std::cerr << "Error: Failed to open Galil connection in copy constructor." << std::endl;
    }
    else {
        std::cout << "Success! Opened Galil connection in copy constructor." << std::endl;
    }
}

// Default destructor. Deallocate memory and close Galil connection.
Galil::~Galil() {
    GResult = Functions->GClose(g); // Close the Galil connection
    delete Functions; // Deallocate the EmbeddedFunctions object
    delete[] ReadBuffer; // Deallocate the ReadBuffer
    Functions = nullptr; // Set pointer to null after deletion
}

// Function for calling EmbeddedFunctions GCommand function to simplify function call within
// Galil functions
void Galil::SendGCommand(const std::string& command) {
    GResult = Functions->GCommand(g, command.c_str(), ReadBuffer, sizeof(ReadBuffer), NULL);
}

// Write to all 16 bits of digital output, 1 command to the Galil
void Galil::DigitalOutput(uint16_t value) {
    std::cout << "Entered DigitalOutput function" << std::endl;
    uint8_t MSB = (uint8_t)((value & 0xFF00) >> 8); // Extract the most significant byte
    uint8_t LSB = (uint8_t)(value & 0x00FF); // Extract the least significant byte
    std::string command = "OP " + std::to_string(LSB) + "," + std::to_string(MSB) + ";"; // Construct the OP command
    SendGCommand(command); // Send the command
    if (GResult != 0) {
        std::cerr << "Error: Failed to execute DigitalOutput command: " << command << std::endl;
    }
    else {
        std::cout << "DigitalOutput executed" << std::endl;
    }

}

// Write to one byte, either high or low byte, as specified by user in 'bank' 0 = low, 1 = high
void Galil::DigitalByteOutput(bool bank, uint8_t value) {
    std::bitset<8> bits(value); // Convert the byte value to binary
    std::string value_str = bits.to_string(); // Get the string representation of the binary value
    bool totalSuccess = 0; // Initialize the success flag
    // Iterate over each bit in the byte
    for (int i = static_cast<int>(value_str.length()) - 1; i >= 0; i--) {
        int j = i;
        if (bank) { // Adjust index if writing to the high byte
            j += 8;
        }
        bool bitValue = (value_str[7 - i] == '1'); // Convert the character to a boolean
        DigitalBitOutput(bitValue, j); // Write each bit individually
        totalSuccess = totalSuccess || GResult; // Update success flag
    }
    GResult = totalSuccess; // Set the final result
    if (GResult != 0) {
        std::cerr << "Error: DigitalByteOutput failed." << std::endl;
    }
    else {
        std::cout << "Success! DigitalByteOutput executed successfully." << std::endl;
    }
}

// Write single bit to digital outputs. 'bit' specifies which bit
void Galil::DigitalBitOutput(bool val, uint8_t bit) {
    std::string command = "OB " + std::to_string(bit) + "," + std::to_string(val) + ";";
    SendGCommand(command);
    if (GResult != 0) {
        std::cerr << "Error: Failed to execute DigitalBitOutput command: " << command << std::endl;
    }
    else {
        std::cout << "DigitalBitOutput executed" << std::endl;
    }
}

// Return the 16 bits of input data
// Query the digital inputs of the GALIL, See Galil command library @IN
uint16_t Galil::DigitalInput() {
    uint16_t digitalInputs = 0;
    std::string errors;
    for (int i = 0; i < 16; ++i) {
        std::string command = "MG @IN[" + std::to_string(i) + "]"; // Query each bit individually
        memset(ReadBuffer, 0, sizeof(ReadBuffer)); // Clear the ReadBuffer before each command
        SendGCommand(command); // Send the command
        if (GResult != 0) {
            errors += "Error: Failed to execute DigitalInput command for input " + std::to_string(i) + "\n";
        }
        else {
            // Check the response in ReadBuffer for '1' or '0' and update the corresponding bit
            if (ReadBuffer[0] == '1') {
                digitalInputs |= (1 << i); // Set the i-th bit
            }
            else if (ReadBuffer[0] == '0') {
                digitalInputs &= ~(1 << i); // Clear the i-th bit
            }
            else {
                errors += "Error: Unexpected response '" + std::string(ReadBuffer) + "' for input " + std::to_string(i) + "\n";
            }
        }
    }
    if (!errors.empty()) {
        std::cerr << errors << std::endl;
    }
    return digitalInputs; // Return the aggregated digital input value
}

// Read either high or low byte, as specified by user in 'bank'
// 0 = low, 1 = high
// A bank is one byte (8 bits). The low bank (0) is the first 8
// bits (DI0-DI7) and the high bank (1) is the upper 8 bits
// (DI8-DI15).
uint8_t Galil::DigitalByteInput(bool bank) {
    uint16_t input = DigitalInput();
    if (bank) {
        // Extract the upper byte (bits 8-15)
        return (input >> 8) & 0xFF;  // Shift right by 8 bits, then mask the result
    }
    else {
        // Extract the lower byte (bits 0-7)
        return input & 0xFF;  // Mask to get the lower 8 bits
    }
}


// Read single bit from current digital inputs. Above functions may use this function
bool Galil::DigitalBitInput(uint8_t bit) {
    uint16_t input = DigitalInput(); // Get all 16 bits of digital input
    std::bitset<8> bits(input & 0xFF); // Convert the lower byte of the input to binary
    std::string bit_str = bits.to_string(); // Convert the binary to string format
    bool input_bit = (bool)(bit_str[7 - bit]); // Access the specified bit and convert to boolean
    return input_bit;
}


// Check the string response from the Galil to check that the last
// command executed correctly. 1 = successful.
// A successful write indicates that a write command (sending a 
// message to the Galil that does not have a response -- e.g., 
// digitalOutput, analogOutput) has completed without errors.
// This should validate some part of the Galil's response (it is 
// up to you how this is completed) but should validly
// differentiate when a write command has been completed 
// successfully.
// This will be called from your main function (do not call it
// within your implementation functions of this Galil class.
bool Galil::CheckSuccessfulWrite() {
    // Check if ReadBuffer has a question mark anywhere (that is what indicates if something went wrong)
    char* result = strchr(ReadBuffer, '?');

    if ((GResult == 0) && (result == nullptr)) {
        return true;
    }
    else {
        return false;
    }
}


// Read Analog channel and return voltage
float Galil::AnalogInput(uint8_t channel) {
    std::string command = "MG @AN[" + std::to_string(channel) + "];"; // Construct the analog input query command
    SendGCommand(command); // Send the command to the Galil
    if (GResult != 0) {
        std::cerr << "Error: Failed to execute AnalogInput command: " << command << std::endl;
    }
    else {
        std::cout << "AnalogInput executed" << std::endl;
    }
    float analogValue = static_cast<float>(atof(ReadBuffer)); // Convert the response to a float
    return analogValue;
}


// Write to any channel of the Galil, send voltages as 2 decimal place in the command string
void Galil::AnalogOutput(uint8_t channel, double voltage) {
    std::string command = "AO " + std::to_string(channel) + "," + std::to_string(voltage) + ";"; // Construct the analog output command
    SendGCommand(command); // Send the command to the Galil
    if (GResult != 0) {
        std::cerr << "Error: Failed to execute AnalogOutput command: " << command << std::endl;
    }
    else {
        std::cout << "AnalogOutput executed" << std::endl;
    }
}

// Configure the range of the input channel with the desired range code
void Galil::AnalogInputRange(uint8_t channel, uint8_t range) {
    std::string command = "AQ " + std::to_string(channel) + "," + std::to_string(range) + ";"; // Construct the analog input range command
    SendGCommand(command);
    if (GResult != 0) {
        std::cerr << "Error: Failed to execute AnalogInputRange command: " << command << std::endl;
    }
    else {
        std::cout << "AnalogInputRange executed" << std::endl;
    }
}


// Manually set the motor encoder value to zero (encoder channel 0)
void Galil::WriteEncoder() {
    std::string command = "WE 0;";
    SendGCommand(command);
    if (GResult != 0) {
        std::cerr << "Error: Failed to execute WriteEncoder command: " << command << std::endl;
    }
    else {
        std::cout << "WriteEncoder executed" << std::endl;
    }
}


// Read from motor Encoder (encoder channel 0)
int Galil::ReadEncoder() {
    std::string command = "QE 0";
    SendGCommand(command);
    if (GResult != 0) {
        std::cerr << "Error: Failed to execute ReadEncoder command: " << command << std::endl;
    }
    else {
        std::cout << "ReadEncoder executed" << std::endl;
    }
    return atoi(ReadBuffer);
}


// Set the desired setpoint for control loops, counts or counts/sec
// This should set it within the class not on the actual Galil.
void Galil::setSetPoint(int s) {
    setPoint = s;
}

// Gets the current setpoint stored in the class
double Galil::getSetPoint() {
    return setPoint;
}

// Set the proportional gain of the controller used in controlLoop() of Position/SpeedControl
// This should set it within the class not on the actual Galil.
void Galil::setKp(double gain) {
    ControlParameters[0] = gain;
}

// Gets the current proportional gain stored in the class
double Galil::getKp() {
    return ControlParameters[0];
}

// Set the integral gain of the controller used in controlLoop()  of Position/SpeedControl
// This should set it within the class not on the actual Galil.
void Galil::setKi(double gain) {
    ControlParameters[1] = gain;
}

// Gets the current integral gain stored in the class
double Galil::getKi() {
    return ControlParameters[1];
}

// Set the derivative gain of the controller used in controlLoop()  of Position/SpeedControl
// This should set it within the class not on the actual Galil.
void Galil::setKd(double gain) {
    ControlParameters[2] = gain;
}

// Gets the current derivative gain stored in the class
double Galil::getKd() {
    return ControlParameters[2];
}

// Operator overload for '<<' operator. So the user can say cout << Galil;
// This function should print out the output of GInfo and GVersion, with
// two newLines after each.
std::ostream& operator<<(std::ostream& output, Galil& galil) {
    // Need to pass by reference, because we need it to go to the screen using cout, if we pass a copy of output it will not go to the screen
    // Passing by reference means that any changes made to output in this function will be reflected everywhere, including outside the function
    {
        // Get info and print to output
        galil.Functions->GInfo(galil.g, galil.ReadBuffer, G_SMALL_BUFFER);
        output << "GInfo: " << galil.ReadBuffer << std::endl << std::endl;

        // Get version and print to output
        galil.Functions->GVersion(galil.ReadBuffer, G_SMALL_BUFFER); 
        output << "GVersion: " << galil.ReadBuffer << std::endl << std::endl;

        return output;  
    }
}

// Copy assignment operator. This acts in the same way as the copy constructor
Galil& Galil::operator=(const Galil& other) {
    // Check that the object is not being assigned to itself
    if (this != &other) {
        Functions = new EmbeddedFunctions(false); // for sim, add true
        GResult = Functions->GOpen(IPAddress, &g);
        std::cout << "Opened connection in copy constructor" << std::endl;
        ReadBuffer = new char[1024];
        // Copy the members from the source object
        for (int i = 0; i < 3; ++i) {
            ControlParameters[i] = other.ControlParameters[i];
        }
        setPoint = other.setPoint;
    }
    return *this;
}