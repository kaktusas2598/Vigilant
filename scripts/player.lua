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
	    onCollide = function(this, second)
			print(type(this))
			print(type(second))
			--print(this:getX())
			for k, v in pairs(getmetatable(this)) do
				print(k)
				if (k == "__index") then
					for k2, v2 in pairs(v) do
						print(k2)
					end
				end
			end


			--Entity.scale(this, 10, 10)
			--this:scale(10, 10)
			--second:scale(5, 5)
			playSound("jump")
			--if (second:id() == "enemy") then
				--second:damage(10)
                --playSound("ouch")
			--end
            print("Lua: onCollide() listener called!")
		end
    },
	input = {
		onInput = function(this, key)
			print(key)
		end
	},
    HP = 100
-- you can also have comments
}

function testFunc(a, b)
    print ("Lua says hello!"..a..b)
    return a + b
end

-- Available functions
-- Entity.create()
-- Entity.move(x, y)
-- Entity.scale(x, y)
-- Entity.addSprite(id, filename, w, h)

testEntity = Entity.create()
testEntity:move(400, 400)
testEntity:scale(2, 2)
testEntity:addSprite("slime", "assets/sprite/slime.png", 32, 32)
testEntity:addCollider("enemy", 48, 48)
--testEntity:addInput() -- segfault because Input needs physics component duh


--testEntity2 = Entity.create()
--testEntity2:move(100, 220) -- testEntity:move() same as Entity.move(testEntity)
--testEntity2:scale(2, 2)
--testEntity:addSprite("slime", "assets/sprite/slime.png", 32, 32)

entityTable = {}
for i = 1, 100 do
	entityTable[i] = Entity.create()
	entityTable[i]:move(100 + i * 100, 220 + i * 200)
	entityTable[i]:scale(1 * (i % 10), 1 * (i % 10))
	entityTable[i]:addSprite("player", "assets/sprite/player.png", 48, 48)
	entityTable[i]:addCollider("enemy", 48, 48)
	-- entityTable[i]:addRigidBody()

end

mapWidth = getMapWidth()
mapHeight = getMapHeight()
print("Map Width: "..mapWidth.."\nMap Height: "..mapHeight)

-- TODO: Write these wrappers:
-- addAnimation()
-- playAnimation()
-- drawTile()?
-- set position, velocity
-- input handling?
-- REGISTER EVENT LISTENERS!!!
