echo "Killing running ticket service"
ps -ef | grep ticket_service | grep -v grep | awk '{print $2}' | xargs kill

cd build
cmake ..
make
ticket/ticket_service &

curl --data "{\"foo\":99}" http://localhost:8890 &
