import socket

def main():
    UDP_IP = "0.0.0.0" # Listen to all interfaces
    UDP_PORT = 4242

    # Create a UDP socket
    udp_socket = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)

    # Bind the socket to the specified address and port
    try:
        udp_socket.bind((UDP_IP, UDP_PORT))
    except Exception as e:
        print(f"Error binding socket: {e}")
        udp_socket.close()
        return

    print(f"Listening for packets on UDP port {UDP_PORT}...")

    try:
        while True:
            # Receive a packet and its source address
            data, addr = udp_socket.recvfrom(1024)  # Buffer size is 1024 bytes

            # Display the received data and the source address in the terminal
            print(f"Received packet from {addr[0]}:{addr[1]}:")
            print(data.decode('utf-8'))  # Assuming the data is in UTF-8 encoding

    except KeyboardInterrupt:
        print("\nStopping the packet listener.")
        udp_socket.close()

if __name__ == "__main__":
    main()