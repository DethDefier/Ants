//SELECT is where you select where teams spawn at beginning
//PASSIVE is in game and passive
//MOVING is in game and moving
//RECEIVING is in game and receiving a move
enum modes {SELECT, PASSIVE, MOVING, RECEIVING, MOVED};
modes mode = SELECT;
//different statuses of sending/receiving ants
enum moveStates {NOT_MOVING, SELECTING_ANTS, SENDING_ANTS, RECEIVING_ANTS};
moveStates moveState = NOT_MOVING;
//food rarity will be 1/FOOD_RARITY+1
const byte FOOD_RARITY = 5;
//maximum value of switchCount
const byte MAX_SWITCH_COUNT = 25;
//team 0 is blank, 1 is blue, 2 is red
byte team = 0;
//num of ants being moved
byte movingAnts = 0;
//list of the 6 ants on the blink
byte ants[6] = {0,0,0,0,0,0};
//if it has food
bool hasFood = false;
//value used in random()
byte rng;
//value for delaying the switch so switching modes goes smoothly
byte switchCount = 0;
void setup() {
  setColor(dim(GREEN,150));
}
void dispAnts() {
  if (!team == 0) {
    setColor(dim(GREEN,150));
    for (byte i = 0; i<5; i++) {
      if  (ants[i] == 1) {
        setColorOnFace(BLUE, i);
      } else if  (ants[i] == 2) {
        setColorOnFace(RED, i);
      } else if  (ants[i] == 3) {
        setColorOnFace(WHITE, i);
      } else if  (ants[i] == 4) {
        setColorOnFace(YELLOW, i);
      }
    }
  }
  if (hasFood) {
    setColorOnFace(ORANGE,0);
  }
}
void setModePassive() {
  moveState = NOT_MOVING;
  hasFood = false;
  mode = PASSIVE;
  setValueSentOnAllFaces(mode);
  switchCount = MAX_SWITCH_COUNT;
  if (team == 0) {
    randomize();
    rng = random(FOOD_RARITY);
    if (rng == 0) {
      hasFood = true;
    }
  } else if (team == 1) {
    ants[0] = 1;
    ants[1] = 3;
    ants[2] = 1;
  } else if (team == 2) {
    ants[0] = 2;
    ants[1] = 4;
    ants[2] = 2;
  } 
  dispAnts();
}

void setModeSelect() {
  mode = SELECT;
  setValueSentOnAllFaces(mode);
  switchCount = MAX_SWITCH_COUNT;
}

void loop() {
  if (mode == PASSIVE) {
    if (buttonLongPressed()) {
      setModeSelect();
    }
    FOREACH_FACE(f) {
      if ((getLastValueReceivedOnFace(f) == SELECT) && (switchCount == 0) && (!isValueReceivedOnFaceExpired(f))) {
        setModeSelect();
      }
    }
    if ((buttonSingleClicked()) && (!team == 0)) {
    setColor(dim(GREEN,150));
    if (!moveState == SELECTING_ANTS) {
        moveState = SELECTING_ANTS;
          movingAnts = 1;
          if (team == 1) {
            setColorOnFace(BLUE,0);
          } else {
            setColorOnFace(RED,0);
          }
        } else if (moveState == SELECTING_ANTS) {
          if (movingAnts < 7) {
            movingAnts++;
          } else {
            movingAnts = 1;
          }
          for (byte i = movingAnts; i>0; i--) {
            if (movingAnts < 7) {
            if (team == 1) {
                if (movingAnts == 6) {
                  setColor(BLUE);
                } 
                setColorOnFace(BLUE, i-1);
            } else {
                if (movingAnts == 6) {
                  setColor(RED);
                }
                setColorOnFace(RED, i-1);
              }
            } else {
              if (team == 1) {
                setColor(WHITE);
              } else {
                setColor(YELLOW);
              }
            }
          }
        }
    }
    if (buttonDoubleClicked()) {
      if (moveState == SELECTING_ANTS) {
        moveState = NOT_MOVING;
        dispAnts();
      }
    }
  }
  if (mode == SELECT) {
    if (buttonSingleClicked()) {
      if (team < 2) {
        team++;
      } else {
        team = 0;
      }
    }
    if (team == 0) {
      setColor(dim(GREEN,150));
    } else if (team == 1) {
      setColor(BLUE);
    } else {
      setColor(RED);
    }
    if (buttonLongPressed()) {
      setModePassive();
    }
    FOREACH_FACE(f) {
      if ((getLastValueReceivedOnFace(f) == PASSIVE) && (switchCount == 0) && (!isValueReceivedOnFaceExpired(f))) {
        setModePassive();
      }
    }
  }
  if (!switchCount == 0) {
    switchCount--;
  }
}
