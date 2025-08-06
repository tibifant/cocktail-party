#!
ls

echo "Running frontend Tests..."
tmux new-session -d -s server-session 'backend/builds/bin/cocktail-party --run-frontend-tests'
if [ $? -ne 0 ]; then tmux kill-session -t server-session; fi
tmux ls
CI=1 npx playwright test
if [ $? -ne 0 ]; then tmux kill-session -t server-session; exit 1; fi
tmux kill-session -t server-session
if [ $? -ne 0 ]; then exit 1; fi
