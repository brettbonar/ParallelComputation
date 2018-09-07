// int rank, size;
//     int data;
//     MPI_Init(&argc, &argv);
//     MPI_Comm_rank(MCW, &rank); 
//     MPI_Comm_size(MCW, &size); 

//     MPI_Send(&rank,1,MPI_INT,(rank+1)%size,0,MCW);
//     MPI_Recv(&data,1,MPI_INT,MPI_ANY_SOURCE,0,MCW,MPI_STATUS_IGNORE);


//     cout<<"I am "<<rank<<" of "<<size<<"; got a message from "<<data<<endl;

//     MPI_Finalize();

//     return 0;

const MPI = require("mpi-node");

MPI.init();
let rank = MPI.rank();
let size = MPI.size();

MPI.send((rank + 1) % size, { msgType: "test", content: rank });
MPI.recv("test", (data) => console.log(data));