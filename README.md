# musiqhq

This project implements several API endpoints that client applications can use to request music data.

Developer APIs from genius.com are used.

This project uses Microsoft's cpprestsdk to set up low-level efficient controllers that map requests to models present in the code. Request objects are then used to communicate with genius.com endpoints to fetch data and send it to clients.
