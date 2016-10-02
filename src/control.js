
ScriptProxy.signalKeyPress.connect(onKeyPress);
ScriptProxy.signalKeyRelease.connect(onKeyRelease);

//coordinate system 0,0 of playerVehicles[1] is in topleft corner
// angles are degree by default

// before you change here anything, make sure this script is actually loaded

// WARNING SCRIPT MAY BE TAKEN FROM RESOURCE
// ALTERING THIS FILE MAY NOT HAVE ANY EFFECT
// check setupScript in graphicsview.cpp for this issue

var playerVehicles = [];

for(var i = 0; i< gameState.playerCount; ++i)
{
	playerVehicles[i] = graphicsEngine.createVehicleAt(416 + 384*i,-600);
	playerVehicles[i].file = i == 0 ? ':playerVehicle1.svg' :  ':images/purplePlayerVehicle1.svg';
	playerVehicles[i].setCenterOffset();
	playerVehicles[i].diplomacy = 1;
	playerVehicles[i].hitpoints = 10;
	playerVehicles[i].isPlayer = true;
	playerVehicles[i].setMaximumVelocity(50.0,50.0);


	// dynamic properties
	playerVehicles[i].i = i;
	playerVehicles[i].shootCooldown = 0;
	playerVehicles[i].reloadCooldown = 0;
	playerVehicles[i].exhaust = graphicsEngine.createAnimatedItemAt(-48,0, ':images/exhaust.svg');
	playerVehicles[i].exhaust.parentItem = playerVehicles[i];
	playerVehicles[i].exhaust.visible = false;
	playerVehicles[i].shield = graphicsEngine.createAnimatedItemAt(0,0,':images/shield.svg');
	playerVehicles[i].shield.parentItem = playerVehicles[i];
	playerVehicles[i].shield.visible = false;
	playerVehicles[i].shieldDuration = 200;
	playerVehicles[i].wormholeTravelTime = 0;
	playerVehicles[i].control = true; // enable disable user control
}

function startAcceleration(/*Vehicle* */ vehicle)
{
	vehicle.accelerating = true;
	vehicle.exhaust.visible = true;
	soundEngine.play('loop_engine.wav_' +vehicle.i,-1);
}

function stopAcceleration(/*Vehicle* */ vehicle)
{
	vehicle.xAcceleration = 0.0;
	vehicle.yAcceleration = 0.0;
	vehicle.accelerating = false;
	vehicle.exhaust.visible = false;
	soundEngine.stop('loop_engine.wav_'+vehicle.i);
}

function enableShield(/*Vehicle* */ vehicle)
{
	if(vehicle.shieldDuration < 75)
		return;

	vehicle.shield.visible = true;
	vehicle.indestructible = true;
	vehicle.control = false;
	soundEngine.play('loop_shield.wav_' + vehicle.i,-1);
}

function disableShield(/*Vehicle* */ vehicle)
{
	vehicle.shield.visible = false;
	vehicle.indestructible = false;
	vehicle.control = true;
	soundEngine.stop('loop_shield.wav_' + vehicle.i);
}

function startWormholeTravel(/*Vehicle* */ vehicle)
{
	if(vehicle.wormholeState != Vehicle.outside)
		return;

	vehicle.control = false; // disable user control
	stopAcceleration(vehicle);
	vehicle.wormholeTravelTime = 50;
	vehicle.beginWormholeTravel();
	soundEngine.play('vanish.wav');
}

function onKeyPress(key)
	{

	if(!isNullQObject(playerVehicles[0])
		&& playerVehicles[0].control == true && playerVehicles[0].wormholeState == Vehicle.outside)
	switch(key)
	{
		case Qt.Key_Up:
		{
			startAcceleration(playerVehicles[0]);
			break;
		}
		case Qt.Key_Left:
		{
			playerVehicles[0].angularVelocity = -250.0;
			break;
		}
		case Qt.Key_Right:
		{
			playerVehicles[0].angularVelocity = 250.0;
			break;
		}
		case Qt.Key_Space:
		{
			playerVehicles[0].shooting = true;
			break;
		}
		// Key_Enter didn't work
		case Qt.Key_Return:
		{
				startWormholeTravel(playerVehicles[0]);
			break;
		}
		case Qt.Key_Delete:
		{
			enableShield(playerVehicles[0]);
			break;
		}
		default:;
	}

	if(!isNullQObject(playerVehicles[1])
		&& playerVehicles[1].control == true && playerVehicles[1].wormholeState == Vehicle.outside)
	switch(key)
	{
		case Qt.Key_W:
		{
			startAcceleration(playerVehicles[1]);
			break;
		}
		case Qt.Key_A:
		{
			playerVehicles[1].angularVelocity = -250.0;
			break;
		}
		case Qt.Key_D:
		{
			playerVehicles[1].angularVelocity = 250.0;
			break;
		}
		case Qt.Key_F:
		{
				startWormholeTravel(playerVehicles[1]);
			break;
		}
		case Qt.Key_G:
		{
			enableShield(playerVehicles[1]);
			break;
		}
		case Qt.Key_Control:
		{
			playerVehicles[1].shooting = true;
			break;
		}
		default:;
	}
}

function onKeyRelease(key)
{

if(!isNullQObject(playerVehicles[0]))
switch (key)
	{
		case Qt.Key_Up:
		{
				stopAcceleration(playerVehicles[0]);
			break;
		}
		case Qt.Key_Left:
		{
			playerVehicles[0].angularVelocity = 0;
			break;
		}

		case Qt.Key_Right:
		{
			playerVehicles[0].angularVelocity = 0;
			break;
		}
		case Qt.Key_Space:
		{
			playerVehicles[0].shooting = false;
			break;
		}
		case Qt.Key_Delete:
		{
			disableShield(playerVehicles[0]);
			break;
		}
		default:;
	}

if(!isNullQObject(playerVehicles[1]))
switch (key)
	{
		case Qt.Key_W:
		{
			stopAcceleration(playerVehicles[1])
			break;
		}
		case Qt.Key_A:
		{
			playerVehicles[1].angularVelocity = 0;
			break;
		}
		case Qt.Key_D:
		{
			playerVehicles[1].angularVelocity = 0;
			break;
		}
		case Qt.Key_Control:
		{
			playerVehicles[1].shooting = false;
			break;
		}
		case Qt.Key_G:
		{
			disableShield(playerVehicles[1]);
			break;
		}
		default:;
	}
}


