#include "trem.h"
#include <QtCore>

sem_t sem0,sem1, sem2, sem3, sem4, sem5, sem6;

//Construtor dos trens
Trem::Trem(int ID, int x, int y){
    this->ID = ID;
    this->x = x;
    this->y = y;
    velocidade = 200;

    initSemaphores();
}
//Funcoes de inicialização e destruição de semaforos
void initSemaphores(){
    sem_init(&sem0, 0, 0);
    sem_init(&sem1, 0, 1);
    sem_init(&sem2, 0, 0);
    sem_init(&sem3, 0, 1);
    sem_init(&sem4, 0, 0);
    sem_init(&sem5, 0, 0);
    sem_init(&sem6, 0, 0);
}

void destroySemaphores(){
    sem_destroy(&sem0);
    sem_destroy(&sem1);
    sem_destroy(&sem2);
    sem_destroy(&sem3);
    sem_destroy(&sem4);
    sem_destroy(&sem5);
    sem_destroy(&sem6);
}

void Trem::setSpeed(int value){
    {
        std::lock_guard<std::mutex> lock(mtx);
        velocidade = value;
    }
    cv.notify_all();
}


//Função a ser executada após executar trem->START
void Trem::run(){
    std::unique_lock<std::mutex> lock(mtx);
    while(true){
        cv.wait(lock, [this] {return velocidade!=200;});

        switch(ID){
        //Trem 1: Movimento e controle do semáforo
        case 1:
            if(y == 60)
            {
                if(x == 410)
                {
                    sem_wait(&sem0);
                }
                if(x < 430)
                    x += 10;
            }

            if(y < 180)
            {
                if(x == 430 && y == 160)
                {
                    sem_wait(&sem2);
                }
                if(x == 430)
                    y+= 10;
            }

            if(y == 180)
            {
                if(x == 260)
                {
                    sem_post(&sem2);
                }
                if(x > 160)
                    x -= 10;

                if(x == 310)
                    sem_wait(&sem1);

                if(x == 400)
                    sem_post(&sem0);
            }

            if(x == 160 && y > 60)
            {
                y-=10;
                if(y == 160)
                    sem_post(&sem1);
            }
            break;
        //Trem 2: Movimento e controle do semáforo
        case 2:
            if(y == 60)
            {
                if(x == 450)
                    sem_post(&sem0);

                if(x < 700)
                    x += 10;
            }

            if(y < 180)
            {
                if(y == 160 && x == 700)
                    sem_wait(&sem4);

                if(y == 160 && x == 430)
                    sem_post(&sem3);

                if(x == 700)
                    y += 10;
            }

            if(y == 180)
            {
                if(x == 540)
                    sem_post(&sem4);

                if(x == 590){
                    sem_wait(&sem0);
                    sem_wait(&sem3);
                }

                if(x > 430)
                    x -= 10;

                if(x == 450)
                {
                    //sem_wait(&sem0);
                }
            }

            if(x == 430 && y > 60)
            {
                y -= 10;
            }
            break;
        //Trem 3: Movimento e controle do semáforo
        case 3:
            if(y == 180)
            {
                if(x == 270)
                    sem_wait(&sem5);

                if(x == 130)
                    sem_wait(&sem1);

                if(x < 290)
                    x += 10;
            }

            if(y < 300)
            {
                if(x == 290 && y == 200)
                    sem_post(&sem1);
                if(x == 290)
                    y += 10;
            }

            if(y == 300)
            {
                if(x == 270)
                    sem_post(&sem5);

                if(x > 20)
                    x -= 10;
            }

            if(x == 20 && y > 180)
                y -= 10;

            break;
        //Trem 4: Movimento e controle do semáforo
        case 4:

            if(y == 180)
            {
                if(x == 450)
                    sem_post(&sem2);

                if(x == 400)
                    sem_wait(&sem3);

                if(x == 310)
                    sem_post(&sem5);

                if(x == 550){
                    sem_wait(&sem6);
                }


                if(x < 570)
                    x += 10;
            }

            if(y < 300)
            {
                if(x == 570 && y == 200)
                    sem_post(&sem3);

                if(x == 570)
                    y += 10;
            }

            if(y == 300)
            {
                if(x == 310)
                {
                    sem_wait(&sem2);
                    sem_wait(&sem5);
                }

                if(x == 540)
                    sem_post(&sem6);

                if(x > 290)
                    x -= 10;
            }

            if(x == 290 && y > 180)
            {
                y -= 10;
            }

            break;
        //Trem 5: Movimento e controle do semáforo
        case 5:

            if(y == 180)
            {
                if(x == 590)
                    sem_post(&sem6);

                if(x < 840)
                    x += 10;

                if(x == 730)
                    sem_post(&sem4);
            }

            if(y < 300)
            {
                if(x == 840)
                    y += 10;
            }

            if(y == 300)
            {
                if(x == 590)
                {
                    sem_wait(&sem4);
                    sem_wait(&sem6);
                }

                if(x > 570)
                    x -= 10;
            }

            if(x == 570 && y > 180)
                y -= 10;

            break;

        default:
            break;
        }
        //updateGUI com mecanismo de suspensão
        emit updateGUI(ID,x,y);
        cv.wait_for(lock,std::chrono::milliseconds(velocidade));
    }

    destroySemaphores();
}



