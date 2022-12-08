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
-- playMusic("mainTheme")
-- end


testEntity = Entity.create()
testEntity:move(400, 400)
testEntity:scale(2, 2)
testEntity:addSprite("slime", "assets/sprite/slime.png", 32, 32)


testEntity2 = Entity.create()
testEntity2:move(100, 220) -- testEntity:move() same as Entity.move(testEntity)
testEntity2:scale(2, 2)
testEntity:addSprite("slime", "assets/sprite/slime.png", 32, 32)

entityTable = {}
for i = 1, 100 do
    entityTable[i] = Entity.create()
    entityTable[i]:move(100 + i * 100, 220 + i * 200)
	entityTable[i]:scale(2, 2)
    entityTable[i]:addSprite("player", "assets/sprite/player.png", 48, 48)
end
-- TODO: Write these wrappers:
-- addAnimation()
-- playAnimation()
-- drawTile()?
-- set position, velocity
-- input handling?
