Cafe Management & Interaction System - User Manual
Programming Project / C++
This system consists of two applications: one for cafe management and one for client interaction.

Application 1: Cafe Management (cafenea.exe)
This application is used for managing the cafe's menu and orders.

Compilation
In the terminal, navigate to the ppangajati directory (e.g., D:\ppcafea\ppangajati\) and use the following compilation command:

Bash

g++.exe -std=c++11 -I../common_headers main.cpp -o cafenea.exe
Command Breakdown:

g++.exe: The C++ compiler.
-std=c++11: Specifies the C++11 standard.
-I../common_headers: Tells the compiler to look for header files in the common_headers directory (expected to be at D:\ppcafea\common_headers\). This path is relative to the ppangajati directory.
main.cpp: The main source file for this application (located in the current ppangajati folder).
-o cafenea.exe: Specifies the output executable name.
Usage
Run the application from the terminal using the following format:
cafenea.exe <comanda>

Available Commands:

cafenea.exe afiseaza_meniu
Displays the current drink menu.
cafenea.exe adauga_bautura <name> <price>
Adds a new drink to the menu.

cafenea.exe sterge_bautura <name>
Deletes a drink from the menu.

cafenea.exe modifica_pret <name> <new_price>
Modifies the price of an existing drink on the menu.

cafenea.exe afiseaza_comenzi
Displays all orders placed by clients.
Application 2: Client Interaction (client.exe)
This application allows clients to interact with the cafe, browse the menu, manage their shopping cart, and place orders.

Compilation
In the terminal, navigate to the ppclienti directory (e.g., D:\ppcafea\ppclienti\) and use the following compilation command:

Bash

g++.exe -std=c++11 -I../common_headers main.cpp -o client.exe
Command Breakdown:

g++.exe: The C++ compiler.
-std=c++11: Specifies the C++11 standard.
-I../common_headers: Tells the compiler to look for header files in the common_headers directory (expected to be at D:\ppcafea\common_headers\). This path is relative to the ppclienti directory.
main.cpp: The main source file for this application (located in the current ppclienti folder).
-o client.exe: Specifies the output executable name.
Usage
Run the application from the terminal using the following format:
client.exe <comanda>

Available Commands:

client.exe afiseaza_meniu
Displays the drink menu.
client.exe afiseaza_cos
Displays the current contents of the client's shopping cart.
client.exe adauga_in_cos <drink_name> <quantity>
Adds a specified quantity of a drink to the shopping cart.

client.exe sterge_din_cos <drink_name>
Removes a drink from the shopping cart.

client.exe modifica_cantitate <drink_name> <new_quantity>
Modifies the quantity of a drink already in the shopping cart.

client.exe plaseaza_comanda
Places the order with the items currently in the shopping cart. This writes the order information to the comenzi.txt file, which is then visible to the cafe management application.
client.exe afiseaza_total
Displays the total price of the items currently in the shopping cart.
