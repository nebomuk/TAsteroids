

// WARNING SCRIPT IS TAKEN FROM RESOURCE

// all script functions that must be called regulary must be in here

ScriptProxy.signalTimerEvent.connect(mainLoop);
gameState.signalPhaseChanged.connect(phaseChange);

var finalPhase = 5 // the number of levels

// call callback for asteroid initialization
phaseChange(0);

var ufo;
var ufoAppearCountdown = 500;

function mainLoop()
{
	for(var i = 0; i < playerVehicles.length; ++i)
	{
		if(isNullQObject(playerVehicles[i]) == true) // check for wrapped NULL pointer
			continue;

		/*wormhole*/
		if(playerVehicles[i].wormholeTravelTime > 0)
		{
			--playerVehicles[i].wormholeTravelTime;
			if(playerVehicles[i].wormholeTravelTime <= 0)
			{
				randPosX = Math.floor(Math.random() *1600.0);
				randPosY = Math.floor(Math.random() * 1200.0);

				if(isNearPlayers(randPosX,randPosY)) // try next time
				{
					playerVehicles[i].wormholeTravelTime = 1;
					continue;
				}

				playerVehicles[i].px = randPosX;
				playerVehicles[i].py = randPosY;

				playerVehicles[i].endWormholeTravel();
				soundEngine.play('appear.wav');
				playerVehicles[i].control = true;
			}
			continue;
		}

		/*shield*/
		if(playerVehicles[i].indestructible == true)
		{
			playerVehicles[i].shieldDuration-=3; // weaken shield

			if(playerVehicles[i].shieldDuration < 100) // begin to flicker
			playerVehicles[i].shield.visible = playerVehicles[i].shieldDuration % 2;

			if(playerVehicles[i].shieldDuration <= 0)
			{
				playerVehicles[i].shield.visible = false;
				playerVehicles[i].indestructible = false;
				soundEngine.stop('loop_shield.wav_'+i);
			}
		}
		else
		{
			if(playerVehicles[i].shieldDuration < 150)
			++playerVehicles[i].shieldDuration; // charge shield
		}


		var cosAngle = Math.cos(playerVehicles[i].angle*Math.PI/180.0);
		var sinAngle = Math.sin(playerVehicles[i].angle*Math.PI/180.0);

		/*acceleration*/
		if(playerVehicles[i].accelerating == true)
		{
			// update acceleration
			playerVehicles[i].xAcceleration = cosAngle*36.0;
			playerVehicles[i].yAcceleration = sinAngle*36.0;
		}

		/*shooting*/
		if(playerVehicles[i].shootCooldown > 0)
		--playerVehicles[i].shootCooldown;

		if(playerVehicles[i].reloadCooldown > 0)
		--playerVehicles[i].reloadCooldown;

        if(playerVehicles[i].shooting === true
		   && playerVehicles[i].shootCooldown <= 0
           && playerVehicles[i].indestructible === false)
		{
			var projectile =
			graphicsEngine.createCircleVehicleAt(playerVehicles[i].x +cosAngle*54,
										   playerVehicles[i].y +sinAngle*54, /*radius = */ 5.0);
			if(i == 1)
				projectile.file = ':images/purpleBomb.svg';

			soundEngine.play('fire.wav');

			graphicsEngine.appendProjectile(projectile);
			var impulseX = cosAngle*40.0+playerVehicles[i].xVelocity;
			var impulseY = sinAngle*40.0+playerVehicles[i].yVelocity;
			projectile.applyImpulse(impulseX,impulseY);
			projectile.diplomacy = 1; // diplomacy of player
			projectile.isProjectile = true;
            playerVehicles[i].reloadCooldown += 5;
			playerVehicles[i].shootCooldown
			= playerVehicles[i].reloadCooldown >= 14 ? playerVehicles[i].reloadCooldown : 4;
		}
	}

    if(!isNullQObject(ufo) && ufo.wormholeState == Vehicle.OUTSIDE)
	{
		var cosAngle = Math.cos(ufo.angle*Math.PI/180.0);
		var sinAngle = Math.sin(ufo.angle*Math.PI/180.0);
		if(ufo.shootCooldown <= 0)
		{
			var projectile =
			graphicsEngine.createCircleVehicleAt(ufo.x +cosAngle*60,
										   ufo.y +sinAngle*60, /*radius = */ 5.0);
            projectile.file = ":images/turquoiseBomb.svg";
			soundEngine.play('fire.wav');
			graphicsEngine.appendProjectile(projectile);
			var impulseX = cosAngle*40.0+ufo.xVelocity;
			var impulseY = sinAngle*40.0+ufo.yVelocity;
			projectile.applyImpulse(impulseX,impulseY);
			projectile.diplomacy = 3; // diplomacy of player
			projectile.isProjectile = true;
			ufo.shootCooldown = 25;
		}
		else
			--ufo.shootCooldown;

		ufo.xAcceleration = cosAngle*10.0;
		ufo.yAcceleration = sinAngle*10.0;

		++ufo.stayTime;
		if(ufo.stayTime  > 1000) // if it stayed too long and it hasn't been destroyed
		{
			ufo.beginWormholeTravel();
			soundEngine.play('vanish.wav');
		}
	}
	else // isNullQObject(ufo)
	{
		--ufoAppearCountdown;
        if(ufoAppearCountdown <= 0  && !gameState.gameOver)
		{
			ufoAppearCountdown = 1000; // appear approximately every 40s
			appearUfo();
		}
	}

	// uncomment this if no asteroids are spawned, else cpu load --> 100%
    if(graphicsEngine.asteroidCount === 0 && !gameState.gameOver)
    {
		gameState.phase += 1;
    }
}

function appearUfo()
{
	randPosX = 800 + Math.floor(Math.random() *800.0);
	randPosY = -400 -Math.floor(Math.random() * 600.0);
	ufo = graphicsEngine.createUfoAt(randPosX,randPosY);
	ufo.endWormholeTravel();
	ufo.xAcceleration = 10;
	ufo.shootCooldown = 0;
	ufo.stayTime = 0; // time the ufo spends till it decides to vanish in a wormhole
}

function distance(x1,y1,x2,y2)
{
	return Math.sqrt((x2-x1)*(x2-x1) + (y2-y1)*(y2-y1));
}

function isNearPlayers(px,py)
{
	for(var i = 0; i < playerVehicles.length; ++i)
	{
		if(isNullQObject(playerVehicles[i]) == true) // check for wrapped NULL pointer
			continue;

		if(distance(playerVehicles[i].px,playerVehicles[i].py,px,py) < 512.0)
			return true;
	}
	return false;
}

function phaseChange(newPhase)
{
    if(newPhase === finalPhase)
    {
        // make players indestructible

        for(var i = 0; i < playerVehicles.length; ++i)
        {
            if(isNullQObject(playerVehicles[i]) === true) // check for wrapped NULL pointer
                continue;

            playerVehicles[i].indestructible = true

        }

        gameState.gameOver = true;
        // todo set single press to exit on android
        // TODO use 5 different colors


        if(ScriptProxy.os === "android")
        {
            graphicsEngine.showText('Congratulations! You Finished the Game! \n Press BACK to return to menu')
        }
        else if(ScriptProxy.os === "ios")
        {
            graphicsEngine.showText('Congratulations! You Finished the Game!')
        }
        else
        {
            graphicsEngine.showText('Congratulations! You Finished the Game! \n Press ESC to return to menu')
        }
    }
    else
       {
        graphicsEngine.showText('Level ' + (newPhase+1),3000);

        for(i = 0; i< 3; ++i)
		{
			do
			{
				randPosX = 0+  Math.floor(Math.random() *1600.0);
				randPosY = 0;//-400 -Math.floor(Math.random() * 600.0);
			}
			while(isNearPlayers(randPosX,randPosY));

			graphicsEngine.createAsteroidAt(randPosX,randPosY,2.0);

		}
		// restore hitpoints every 2 levels
		if(newPhase % 2 == 0)
		{
			for(i= 0; i< playerVehicles.length; ++i)
				playerVehicles[i].hitpoints = 10;
		}
    }
}

