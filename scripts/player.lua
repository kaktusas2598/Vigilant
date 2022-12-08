-- Entity
player = {
    -- Entity components
    transform = {
         X = 100,
         Y = 250,
         scaleX = 2.0,
         scaleY = 2.0

    },
    physics = {
        mass = 1.0,
        velocityX = 0.0,
        velocityY = 0.0,
        aceclerationX = 0.0,
        accelerationY = 0.0,
        gravityX = 0.0,
        gravityY = 0.0,
        friction = 0.2,
    },
    sprite = {
        filename = "assets/player.png",
        id = "player",
        animation = {
			{
                name = "down",
                row = 0,
                numFrames = 6
            },
            {
                name = "default",
                row = 1,
                numFrames = 6
            },
            {
                name = "up",
                row = 2,
                numFrames = 6
            }
		},
        width = 48,
        height = 48
    },
    collider = {
        type = "player",
        width = 15,
        height = 35,
	    collide = function(this, second)
            print(this)
        end
    },
    input = {},
    HP = 20
-- you can also have comments
}

function testFunc(a, b)
    print ("Lua says hello!"..a..b)
    return a + b
end

-- print(player["sprite"]["animation"])

-- It works! Calling engine from lua!
-- while (true)
-- do
-- playSound("jump")
playMusic("mainTheme")
-- end


testEntity = createEntity()
teleportEntity(testEntity, 400, 400)
addSprite(testEntity, "slime", "assets/sprite/slime.png", 48, 48)


testEntity2 = createEntity()
teleportEntity(testEntity2, 100, 220)
addSprite(testEntity2, "slime", "assets/sprite/slime.png", 48, 48)
-- TODO: Write these wrappers:
-- addAnimation()
-- playAnimation()
-- drawTile()?
-- set position, velocity
-- input handling?
