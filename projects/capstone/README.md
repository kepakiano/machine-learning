# Machine Learning Engineer Nanodegree

This is the repository for my capstone project. In order to compile it you will need cmake, make, a GCC version that can compile C++11, the Simple Directmedia Layer library SDL1.2, SDL\_ttf2.0 and SDL\_image1.2. Furthermore, sqlite3 is needed to store and load data.

The project uses the SQLite library SQLiteCpp (https://github.com/SRombauts/SQLiteCpp) as git submodule located in game/game\_lib. Recent versions of git should clone the repository into the appropriate folder. If that is not the case, plus run 'git submodule update --recursive --remote'.

The database containing the final results is called asteroids.db3. Please copy this to the ReinforcementLearningTraining subdirectory of your build folder.

You can run three different executables:

AsteroidsGUI without argument will let you play the game. Use the arrow keys to control the ship and space to shoot.
If you provide an argument in form of a number, the game will load the appropriate test case from the database and showcase what the agent has learned. The best performing agent can be started using ./AsteroidsGUI 406.

ReinforcementLearningTraining is the executable that creates the database (if it's not present yet) and trains the algorithm with a fixed set of parameters. Use -h to see a list of parameters.

ReinformentLearningTesting without any argument will pull every test case from the database, run it 100 times and write the results back. With an argument - like AsteroidsGUI - it will only run one test case.
