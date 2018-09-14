const klyng = require("klyng");
 
function main() {
  let size = klyng.size();
  let rank = klyng.rank();

  klyng.send({
    to: (rank + 1) % size,
    data: rank,
    subject: "Rank"
  });

  let result = klyng.recv({ subject: "Rank" });
  
  console.log("My Rank:", rank, ", Data Received:", result);

  klyng.end();
}
 
klyng.init(main);
