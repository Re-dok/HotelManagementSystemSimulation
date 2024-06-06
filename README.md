# HotelManagementSystemSimulation
This project simulates a hotel management system using various operating system concepts learned such as Linux commands, process creation, inter-process communication, multithreading, and synchronization mechanisms. The system includes different entities like an Admin, a Hotel Manager, Tables, Customers, and Waiters, all represented as separate processes.

## Project Description

### System Overview
The system simulates a hotel environment with the following components:
- **Admin Process**: Initiates and oversees the entire system, responsible for termination signals.
- **Hotel Manager Process**: Manages total earnings, oversees waiter operations, and handles system termination.
- **Table Processes**: Represents tables in the hotel, with each table process capable of creating multiple customer processes.
- **Customer Processes**: Created by table processes, representing customers sitting at a table.
- **Waiter Processes**: Each waiter is assigned to a specific table and is responsible for taking orders and calculating bills.

### Key Features
- Each table process can handle a maximum of five concurrent customer processes.
- A waiter with ID `x` attends only to table number `x`.
- Separate terminal execution for each table and waiter process.
- Interprocess communication is used for various interactions between processes.
- Synchronization and concurrency control using POSIX threads and locks.
