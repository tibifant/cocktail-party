#!
echo "Installing playwright..."
npm init -y
npm install -D @playwright/test@1.54.2
npx playwright install-deps
npx playwright install chromium

echo "Starting Server..."
if [ $? -ne 0 ]; then exit 1; fi
tmux new-session -d -s server-session 'backend/builds/bin/cocktail-party --run-frontend-tests'
if [ $? -ne 0 ]; then tmux kill-session -t server-session; exit 1; fi
echo "Running Tests..."
CI=1 npx playwright test
if [ $? -ne 0 ]; then tmux kill-session -t server-session; exit 1; fi
#echo "Stopping Server..."
tmux kill-session -t server-session