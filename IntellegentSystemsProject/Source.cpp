#include <iostream>
#include <cmath>
#include <time.h>
using namespace std;

#define mapLength 2000
#define numOfParticles 50
#define stepSize 1
#define startPosition 90
#define startDirection 1 
#define numOfIteration 1000
# define M_PI           3.14159265358979323846  /* pi */


//each array element has its own sensor value (which is a char)
double map[mapLength];

struct Particle {
    int position;
    int direction; // 0 means left and 1 means right
    double weight;
    double cumulativeWeight;
};

//set of particles
Particle particles[numOfParticles];
Particle selectedParticles[numOfParticles];

struct Plan {
    int planPosition = startPosition;
    int planDirection = startDirection;
} plan;


void calcCumulativeWeights() {
    for (int i = 1; i <= numOfParticles; i++)
    {
        particles[i].cumulativeWeight = particles[i].weight;
        for (int j = 1; j < i; j++)
        {
            particles[i].cumulativeWeight += particles[j].weight;
        }
    }
    cout << endl;
}

//inital values
void setParticles() {
    for (int i = 1; i <= numOfParticles; i++)
    {
        particles[i].position = rand() % mapLength + 1;
        particles[i].direction = rand() % 2; //Either 0 or 1
        particles[i].weight = 1.0 / numOfParticles;
    }
}


void printParticlesDetails() {
    cout << "particles positions: ";
    for (int i = 1; i <= numOfParticles; i++)
    {
        cout << particles[i].position << " ";
    }
    cout << endl;
    cout << "particles directions: ";
    for (int i = 1; i <= numOfParticles; i++)
    {
        cout << particles[i].direction << " ";
    }
    cout << endl;
    cout << "particles weights: ";
    for (int i = 1; i <= numOfParticles; i++)
    {
        cout << particles[i].weight << " ";
    }
    cout << endl << endl;

}

//i used charachter as sensor value
//void setMap() {
//    for (int i = 1; i <= mapLength; i++)
//    {
//        map[i] = (rand() % 26) + 'A';
//    }
//}

// here the code from instructor 
/*double Map[], int size*/
void generateMap() {
    int j = 0;
    for (double k = 0.001; k <= 1.0; k += 0.001)
    {
        double y;
        double i = 2 * M_PI * k;
        y = cos(i) + 0.5 * cos(3 * i + 0.23) + 0.5 * cos(5 * i - 0.4) + 0.5 * cos(7 * i + 2.09) + 0.5 * cos(9 * i - 3);
        map[j] = y * 0.5;
        j++;
    }
}



void printMap() {
    // printParticlesDetails();
    for (int i = 1; i <= mapLength; i++)
    {
        cout << map[i] << "  " << "|" << " ";
    }
    cout << endl;

    for (int i = 1; i <= mapLength; i++)
    {

        cout << i;
        if (i < 10)
            cout << " ";
        cout << " " << "|" << " ";
    }
    cout << endl;

    // to display the sensor position :

    for (int i = 1; i < plan.planPosition; i++) {
        cout << "     ";
    }
    cout << "R" << endl;

    // to dispaly the number of particles in each position :
    int pNum;
    for (int i = 1; i <= mapLength; i++)
    {
        pNum = 0;
        for (int j = 1; j <= numOfParticles; j++)
        {
            if (particles[j].position == i)
                pNum++;
        }

        cout << pNum;
        if (pNum <= 9)
            cout << " ";
        cout << " | ";

    }
    cout << endl << endl << endl;
}

void printselectedParticles() {
    cout << "\n\nweights of selected particles:" << endl;
    for (int i = 1; i <= numOfParticles; i++)
    {
        cout << selectedParticles[i].weight << " ";
    }
    cout << endl << endl;
}

void particleFilter() {
    calcCumulativeWeights();
    int sumOfWeights = 0;
    // Random numbers generated between 0 and 1:
    double randNum;
    for (int i = 1; i <= numOfParticles; i++)
    {
        randNum = (double)rand() / RAND_MAX;
        for (int j = 1; j <= numOfParticles; j++)
        {
            //1. select particle
            if (randNum <= particles[j].cumulativeWeight) {
                selectedParticles[i] = particles[j];
                break;
            }
        }

        //2. update position
        // rotate the particle if needed
        if ((selectedParticles[i].position == mapLength && selectedParticles[i].direction == 1)
            || (selectedParticles[i].position == 1 && selectedParticles[i].direction == 0))
        {
            if (selectedParticles[i].direction == 1)
                selectedParticles[i].direction = 0; // Move it's direction to left
            else
                selectedParticles[i].direction = 1; // Move it's direction to right
        }

        if (selectedParticles[i].direction == 1) {
            selectedParticles[i].position += stepSize;
        }
        else {
            selectedParticles[i].position -= stepSize;
        }

        //3. update weight
        selectedParticles[i].weight = 26 - abs(map[plan.planPosition] - map[selectedParticles[i].position]);
        sumOfWeights += selectedParticles[i].weight;
    }

    cout << endl;

    //4. normalized weight
    for (int i = 1; i <= numOfParticles; i++)
    {
        selectedParticles[i].weight /= sumOfWeights;
    }

    //5. set the new selectedParticles[] as particles[] 
    for (int i = 1; i <= numOfParticles; i++)
    {
        particles[i] = selectedParticles[i];
    }
}

void move() {
    if ((plan.planPosition == mapLength && plan.planDirection == 1) || (plan.planPosition == 1 && plan.planDirection == 0))
    {
        if (plan.planDirection == 1)
            plan.planDirection = 0; // Move it's direction to left
        else
            plan.planDirection = 1; // Move it's direction to right


        for (int i = 1; i <= numOfParticles; i++)
        {
            // Update particles direction (rotate)
            if (particles[i].direction == 0)
                particles[i].direction = 1;
            else
                particles[i].direction = 0;
        }
    }

    if (plan.planDirection == 1) {
        plan.planPosition += stepSize;
    }
    else {
        plan.planPosition -= stepSize;
    }
    particleFilter();

}


int printOutput() {
    double mean = 0, standardDeviation = 0;
    for (int i = 1; i <= numOfParticles; i++)
    {
        mean += particles[i].position;
    }
    mean /= numOfParticles;

    for (int i = 1; i <= numOfParticles; i++)
    {
        standardDeviation += pow(particles[i].position - mean, 2);
    }

    standardDeviation /= numOfParticles;
    standardDeviation = sqrt(standardDeviation);

    cout << "plan position: " << plan.planPosition << "\t Mean: " << mean << "\t Standard deviation: " << standardDeviation << endl << endl;

    if (standardDeviation == 0)
        return 1;
    else
        return 0;

}

int main() {
    srand((unsigned)time(NULL));
    setParticles();
    /*setMap();*/
    //double map[mapLength];
/*map, mapLength*/
    generateMap();
   /* printMap();
    printOutput();*/

    int i = 0;
    while (i < numOfIteration)
    {
        cout << "-----------------------------------------------------------------------" << endl;
        move();
       /* printMap();*/
        if (printOutput())
            return 0;
        i++;
    }
    return 0;
}