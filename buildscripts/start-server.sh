#!

echo "Start nginx..."
nginx
# tmux ls
ps -ax | grep nginx
echo "Start server..."
./cocktail-party
