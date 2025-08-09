# Cocktail Party
## A random cocktail generator

Do you want to have a fun evening, but don't know what to drink?
Cocktail Party has your back!
- Enjoy inspiring recipes.
- Add new cocktails to your collection.
- Edit and Remove the one's that don't strike your taste.

## Find the website

Open [mlindne2-toolbox.rewoit.de/](https://mlindne2-toolbox.rewoit.de/)

## Run the application locally

### Build the application:

Dependency: [asio](https://think-async.com/Asio/) (on Linux: `sudo apt install asio`)

On Windows:
- Run create_project.bat
- Open the solution (cocktail-party.sln) in Visual Studio 2022
- Build the solution (Build > Build Solution)

On Linux (clang-18):
- Run buildscripts/debug.sh (Debug) / buildscripts/release.sh (Release)

### Run the application:

On windows:
- Execute builds/bin/cocktail-partyD.exe (Debug) / builds/bin/cocktail-party.exe (Release)

On Linux:
- Execute backend/builds/bin/cocktail-party

Queries will be processed on port 61919.

For testing:
- Backend: Run the application with `--run-tests`.
- End-to-End: Install [playwright](https://playwright.dev/docs/intro#installing-playwright), then run the application with `--run-frontend-tests` and run `npx playwright test`.

## Build the container image

Make sure docker is installed and running.

- Run `docker build -t standalone -f Dockerfile-standalone .`

## Run the container locally

- Run `docker run -it standalone`
