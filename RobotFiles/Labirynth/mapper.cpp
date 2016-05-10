#define FIELD_NWALL 0x01
#define FIELD_SWALL 0x02
#define FIELD_WWALL 0x04
#define FIELD_EWALL 0x08
#define FIELD_WALLS 0x0f
#define FIELD_UNMAPPED 0x10
#define FIELD_MAPPED 0x20
#define FIELD_START 0x40
#define FIELD_GOAL 0x80
#define FIELD_STATE 0xf0

//Remove this in future
typedef unsigned char uint8_t;

struct LabirynthField {
	uint8_t distance;
	uint8_t status;
};
LabirynthField map[8][8];
bool goalFound = false;

// Globals and functions that should be declared elsewhere
#define DIRECTION_NORTH 0x01
#define DIRECTION_SOUTH 0x02
#define DIRECTION_WEST 0x04
#define DIRECTION_EAST 0x08
void moveOneField(uint8_t direction) {}
uint8_t scanWalls() {}

// Helper functions

void updateDistances(uint8_t x, uint8_t y, uint8_t distance) {
	// Just write the distance for yet unmapped fields
	if(map[x][y].status & FIELD_UNMAPPED) {
		map[x][y].distance = distance;
		return;
	}

	// If field's distance is smaller or equal to new one, go no further
	if(map[x][y].distance <= distance) {
		return;
	}

	map[x][y].distance = distance;

	uint8_t walls = map[x][y].status & FIELD_WALLS;
	if(!(walls & FIELD_NWALL)) {
		updateDistances(x, y+1, distance + 1);
	}
	if(!(walls & FIELD_SWALL)) {
		updateDistances(x, y-1, distance + 1);
	}
	if(!(walls & FIELD_WWALL)) {
		updateDistances(x-1, y, distance + 1);
	}
	if(!(walls & FIELD_EWALL)) {
		updateDistances(x+1, y, distance + 1);
	}
}

/**
 * Map the labirynth.
 * Robot always begins facing [logical] north.
 * @param {bool} westernCorner - whether we're in the western corner (eastern otherwise)
 * @param {bool} southernCorner - whether we're in the southern corner (northern otherwise)
 */
void mapLabirynth(bool westernCorner = true, bool southernCorner = true) {
	uint8_t currentX = (westernCorner ? 0 : 7);
	uint8_t currentY = (southernCorner ? 0 : 7);
	uint8_t currentDistance = 0;

	// Clear the map
	for(int i = 0; i < 8; ++i) {
		for(int j = 0; j < 8; ++j) {
			map[i][j].distance = 0xff;
			map[i][j].status = FIELD_UNMAPPED;
			if(i == 0) {
				map[i][j].status |= FIELD_SWALL;
			} else if(i == 7) {
				map[i][j].status |= FIELD_NWALL;
			}
			if(j == 0) {
				map[i][j].status |= FIELD_WWALL;
			} else if(j == 7) {
				map[i][j].status |= FIELD_EWALL;
			}
		}
	}

	// Write known map fields - goal and start
	map[3][3].status = map[3][4].status = map[4][3].status = map[4][4].status = FIELD_GOAL | FIELD_UNMAPPED;
	map[currentX][currentY].status = FIELD_START;

	while(1) {
		uint8_t currentFieldWalls = 0x00;
		// If current field wasn't yet mapped, do it now
		if(map[currentX][currentY].status & FIELD_UNMAPPED) {
			// Scan walls and verify the reading
			currentFieldWalls = scanWalls();
			bool scanError = false;
			if(currentX == 0 && !(currentFieldWalls & FIELD_SWALL)) {
				scanError = true;
			}
			if(currentX == 7 && !(currentFieldWalls & FIELD_NWALL)) {
				scanError = true;
			}
			if(currentX == 0 && !(currentFieldWalls & FIELD_WWALL)) {
				scanError = true;
			}
			if(currentX == 7 && !(currentFieldWalls & FIELD_EWALL)) {
				scanError = true;
			}
			if(scanError) {
				//printf("SCAN ERROR\n");
				break;
			}

			// Modify field status
			map[currentX][currentY].status &= !FIELD_UNMAPPED;
			map[currentX][currentY].status |= FIELD_MAPPED;
			map[currentX][currentY].status |= currentFieldWalls;

			// Update this field's and its neighbours' distances (recursive!)
			updateDistances(currentX, currentY, currentDistance);
			currentDistance++;

			if((currentX == 3 || currentX == 4) && (currentY == 3 || currentY == 4)) {
				goalFound = true;
			}
		} else {
			// Else, just read it's wall mapping
			currentFieldWalls = map[currentX][currentY].status & FIELD_WALLS;
		}

		// Decide where to go next
		// Initialize interest variables
		uint8_t interestNorth, interestSouth, interestEast, interestWest;
		interestNorth = interestSouth = interestEast = interestWest = 0x80;

		// Basic heuristic - try to equalize distance from goal in both directions
		if(currentX <= 3 && currentY <= 3) {
			// SW quarter
			interestNorth += 0x40;
			interestEast += 0x40;
			interestSouth -= 0x40;
			interestWest -= 0x40;
			if(currentY <= currentX) {
				interestNorth += 0x20;
				interestWest += 0x10;
			} else {
				interestEast += 0x20;
				interestSouth += 0x10;
			}
		} else if(currentX <= 3 && currentY >= 4) {
			// SE quarter
			interestNorth += 0x40;
			interestWest += 0x40;
			interestSouth -= 0x40;
			interestEast -= 0x40;
			if(currentY <= (7 - currentX)) {
				interestNorth += 0x20;
				interestEast += 0x10;
			} else {
				interestWest += 0x20;
				interestSouth += 0x10;
			}
		} else if(currentX <= 3 && currentY <= 3) {
			// NW quarter
			interestSouth += 0x40;
			interestEast += 0x40;
			interestNorth -= 0x40;
			interestWest -= 0x40;
			if((7 - currentY) <= currentX) {
				interestSouth += 0x20;
				interestWest += 0x10;
			} else {
				interestEast += 0x20;
				interestNorth += 0x10;
			}
		} else if(currentX <= 3 && currentY <= 3) {
			interestSouth += 0x40;
			interestWest += 0x40;
			interestNorth -= 0x40;
			interestEast -= 0x40;
			if((7 - currentY) <= (7 - currentX)) {
				interestSouth += 0x20;
				interestEast += 0x10;
			} else {
				interestWest += 0x20;
				interestNorth += 0x10;
			}
		}

		// Cross out blocked or already mapped adhering fields
		if(currentFieldWalls & FIELD_NWALL || map[currentX][currentY+1].status & FIELD_MAPPED) {
			interestNorth = 0;
		}
		if(currentFieldWalls & FIELD_SWALL || map[currentX][currentY-1].status & FIELD_MAPPED) {
			interestSouth = 0;
		}
		if(currentFieldWalls & FIELD_WWALL || map[currentX-1][currentY].status & FIELD_MAPPED) {
			interestWest = 0;
		}
		if(currentFieldWalls & FIELD_EWALL || map[currentX+1][currentY].status & FIELD_MAPPED) {
			interestEast = 0;
		}

		// Nothing more to map here, move back one field (assign minimal non-zero interest)
		if(interestNorth | interestSouth | interestWest | interestEast == 0) {
			if(!(currentFieldWalls & FIELD_NWALL) && map[currentX][currentY+1].distance < currentDistance - 1) {
				interestNorth = 0x01;
			}
			if(!(currentFieldWalls & FIELD_SWALL) && map[currentX][currentY-1].distance < currentDistance - 1) {
				interestSouth = 0x01;
			}
			if(!(currentFieldWalls & FIELD_WWALL) && map[currentX-1][currentY].distance < currentDistance - 1) {
				interestWest = 0x01;
			}
			if(!(currentFieldWalls & FIELD_EWALL) && map[currentX+1][currentY].distance < currentDistance - 1) {
				interestEast = 0x01;
			}
		}

		// Go to most preferred direction (if some are equal, use NSWE order)
		if(interestNorth >= interestSouth && interestNorth >= interestWest && interestNorth >= interestEast) {
			moveOneField(DIRECTION_NORTH);
			currentY++;
		} else if(interestSouth >= interestNorth && interestSouth >= interestWest && interestSouth >= interestEast) {
			moveOneField(DIRECTION_SOUTH);
			currentY--;
		} else if(interestWest >= interestNorth && interestWest >= interestSouth && interestWest >= interestEast) {
			moveOneField(DIRECTION_WEST);
			currentX--;
		} else if(interestEast >= interestNorth && interestEast >= interestSouth && interestEast >= interestWest) {
			moveOneField(DIRECTION_EAST);
			currentX++;
		}

		if(goalFound && currentX == (westernCorner ? 0 : 7) && currentY == (southernCorner ? 0 : 7)) {
			break;
		}
	}
}
