(
clients=8
messages_each=10

for i in $(seq 1 $clients); do
  npx cross-env CLIENTS=$clients MESSAGES_EACH=$messages_each jest &
done
wait
)