#include "EmbeddedFunctions.h"

EmbeddedFunctions::EmbeddedFunctions() {
    try {
        // Initialise the TcpClient
        this->GALIL = gcnew TcpClient();

        // Initialise new data variables
        this->SendBufferSize = 64;
        this->RecvBufferSize = 2048;
        this->NoDelay = true;
        this->SendData = gcnew array<Byte>(SendBufferSize);
        this->ReceiveData = gcnew array<Byte>(RecvBufferSize);

        // Default timeout values for sending and receiving
        this->SendTimeout = 500;
        this->ReceiveTimeout = 500;

        Console::WriteLine("EmbeddedFunctions constructor executed, object created");
    }
    catch (Exception^ ex) {
        // Handle exceptions during initialization
        throw gcnew Exception("Failed to initialize EmbeddedFunctions: " + ex->Message);
    }
}

EmbeddedFunctions::~EmbeddedFunctions() {
    try {
        // Confirm that the TcpClient is initialised and connected
        if (this->GALIL != nullptr && this->GALIL->Connected) {
            // Close the TcpClient connection
            this->GALIL->Close();
        }
        Console::WriteLine("Destructor executed correctly, connection to PLC closed.");
    }
    catch (Exception^ ex) {
        // Handle exceptions during the destruction process
        throw gcnew Exception("Error occurred while closing the connection in destructor: " + ex->Message);
    }
}

/**
 * Open a connection to a Galil Controller.
 *
 * @param address Null-terminated address string. Use direct connection (-d) to connect to hardware or simulator (e.g., "192.168.0.120 -d").
 * @param port Integer value for the port to connect to.
 *
 * @throws Exception if one occurs.
 */
void EmbeddedFunctions::GOpen(String^ address, const int port) {
    IPAddress = address;
    Port = port;

    // Ensure no existing connection
    if (this->GALIL != nullptr && this->GALIL->Connected) {
        throw gcnew Exception("An active connection already exists.");
    }

    try {

        // Create a new TcpClient and attempt to connect
        this->GALIL = gcnew TcpClient(IPAddress, Port);

        // Network configurations
        this->GALIL->NoDelay = NoDelay;
        this->GALIL->SendTimeout = SendTimeout;
        this->GALIL->ReceiveTimeout = ReceiveTimeout;

        // Get the network stream for communication
        this->GALILStream = this->GALIL->GetStream();

        Console::WriteLine("Connection established to PLC at: " + IPAddress + ":" + Port);
    }
    catch (SocketException^ socketEx) {
        throw gcnew Exception("Failed to connect to PLC. Socket exception: " + socketEx->Message);
    }
    catch (Exception^ ex) {
        throw gcnew Exception("Failed to open connection: " + ex->Message);
    }
}

/**
 * Closes a connection to a Galil Controller.
 *
 * @throws Exception if one occurs.
 */
void EmbeddedFunctions::GClose() {
    try {
        // Check if there's an active connection
        if (this->GALIL != nullptr && this->GALIL->Connected) {
            if (this->GALILStream != nullptr) {
                this->GALILStream->Close();  // Close the network stream
                this->GALILStream = nullptr;  // Set to nullptr after closing
                Console::WriteLine("Network stream closed.");
            }

            // Close the TcpClient after closing the stream
            this->GALIL->Close();
            this->GALIL = nullptr;  // Set TcpClient to nullptr
            Console::WriteLine("Connection to PLC has been successfully terminated.");
        }
        else {
            throw gcnew Exception("No active connection to close.");
        }
    }
    catch (Exception^ ex) {
        throw gcnew Exception("Error during closing the connection: " + ex->Message);
    }
}

/**
 * Performs a *command-and-response* transaction on the connection.
 *
 * @param command Null-terminated command string to send to the controller.
 * @return The response from the Galil.
 * @throws Exception if one occurs.
 */
String^ EmbeddedFunctions::GCommand(String^ command) {
    try {
        // Append semicolon if not present
        if (!command->EndsWith(";")) {
            command += ";";
        }

        // Convert the command to bytes
        SendData = Encoding::ASCII->GetBytes(command);

        // Ensure the connection and stream are properly initialized
        if (this->GALIL == nullptr || !this->GALIL->Connected || this->GALILStream == nullptr) {
            throw gcnew Exception("No active connection. Command cannot be sent.");
        }

        // Write the command to the stream
        this->GALILStream->Write(SendData, 0, SendData->Length);
        Console::WriteLine("Command sent to PLC: " + command);

        // Wait for the PLC to process the command
        Threading::Thread::Sleep(2000);

        // Read the response from the stream
        int bytesRead = this->GALILStream->Read(ReceiveData, 0, ReceiveData->Length);

        // Convert the response to a string
        String^ response = Encoding::ASCII->GetString(ReceiveData, 0, bytesRead);
        Console::WriteLine("Response from PLC: " + response);

        return response;
    }
    catch (Exception^ ex) {
        throw gcnew Exception("Error during command execution: " + ex->Message);
    }
}
