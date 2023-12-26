# export DB_HOST=127.0.0.1
# export DB_PORT=3360
# export DB_LOGIN=stud
# export DB_PASSWORD=stud
# export DB_DATABASE=archdb
# export CACHE=127.0.0.1:6379

# ./build/user_service

export DB_HOST=127.0.0.1
export DB_PORT=3360
export DB_LOGIN=stud
export DB_PASSWORD=stud
export DB_DATABASE=archdb
export CACHE=127.0.0.1:6379
export QUEUE_HOST=127.0.0.1:9092
export QUEUE_TOPIC=event_server
export QUEUE_GROUP_ID=0

./build/user_service


