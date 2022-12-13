-- Available functions from Engine

-- Entity functions:
-- Entity.create(), use create() wrapper which adds entity to entities global table which Lua will use
-- Entity.move(x, y)
-- Entity.scale(x, y)
-- Entity.addSprite(id, filename, w, h)
-- Entity.setAbsolutePosition(isAbsolute): takes boolean argument
-- Entity.addPhysics(mass, frictionCoefficient) : friction from 0.00 to 1.00
-- Entity.addCollider(type, width, height)
-- Entity.setColliderListener(funcName)
-- Entity.addInput()
-- Entity.setInputListener(funcName)
-- Entity.addButton()
-- Entity.setButtonListener(funcName)
-- Entity.id()

-- General Functions:
-- playSound(id)
-- playMusic(id)
-- getMapWidth()
-- getMapHeight()
-- getScreenWidth()
-- getScreenHeight()
-- changeState() - currently sets the next state in state machine

-- Globals
entities = {} -- All Lua Managed entities will be here sorted by id

function create()
	entity = Entity.create()
	entities[entity:id()] = entity
	return entity
end

function define(data, tag)
	entity = create()
	for k, v in pairs(data) do
		if k == "transform" then
			entity:move(v['X'], v['Y'])
			entity:scale(v['scaleX'], v['scaleY'])
		end
		if k == "sprite" then
			-- Assume required properties are set for now
			entity:addSprite(v['id'], v['filename'], v['width'], v['height'])
			for k2,v2 in pairs(v['animation']) do
				entity:addAnimation(v2['name'], v2['row'], v2['numFrames'])
			end
		end
		if k == "input" then
			entity:addInput()
			entity:setInputListener("onInput")
		end
		if k == "physics" then
			entity:addPhysics(v['mass'], v['friction'])
		end
		if k == "collider" then
			entity:addCollider(v['type'], v['width'], v['height'])
			entity:setCollideListener("onCollide")
		end
	end
	entities[entity:id()] = entity
	return entity
end


-- Player event listeners
function onCollide(thisId, secondId)
	print("Lua: onCollide() called")
	--print("Player X: "..player:getX())
	--print("Player Y: "..player:getY())
	entities[secondId]:scale(0.5, 0.5)
	print(entities)
	-- if object:type() == "enemy" then
		-- player:damage(10)
	-- end
end

-- TODO: SDL key codes??
function onInput(thisId, key)
	print("Lua: Key pressed: "..key)
	-- 102 - 'F'
	if key == 102 then
		playSound("jump")
		testEntity = create()
		testEntity:move(entities[thisId]:getX(), entities[thisId]:getY())
		testEntity:addSprite("bullet", "assets/projectiles/bullet1.png", 10, 10)
		testEntity:addProjectile(entities[thisId], 150, 1000)
	end
end

-- back to Main Menu callback and definition
function onMainMenuClick(thisId)
	-- TODO: make this binding better, accept state id or ScreenState enum
	changeState()
end
mainMenu = create()
-- TODO: Static sprite positioning
mainMenu:move((getScreenWidth() - 200)/2, 0)
mainMenu:addSprite("mainmenubutton", "", 200, 80)
mainMenu:setAbsolutePosition(true)
mainMenu:addButton()
mainMenu:setButtonListener("onMainMenuClick")


-- Entity
playerTable = {
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
        friction = 0.4,
    },
    sprite = {
        filename = "assets/sprite/player.png",
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
	    listener = "onCollide"
    },
	input = {
		listener = "onInput"
	},
    HP = 100
}

player = define(playerTable)

mapWidth = getMapWidth()
mapHeight = getMapHeight()
screenWidth = getScreenWidth()
screenHeight = getScreenHeight()

print("Map Width: "..mapWidth.."\nMap Height: "..mapHeight)
print("Screen Width: "..screenWidth.."\nScreen Height: "..screenHeight)

print("Player ID: "..player:id())
print("Player X: "..player:getX())
print("Player Y: "..player:getY())


testEntity = create()
testEntity:move(400, 400)
testEntity:scale(2, 2)
testEntity:addSprite("slime", "assets/sprite/slime.png", 32, 32)
testEntity:addAnimation("default", "0", "4")
testEntity:setAnimation("default")
testEntity:addCollider("enemy", 48, 48)
testEntity:addPhysics(4.0, 0.3)
print("Slime ID: "..testEntity:id())

-- Testing deleting entity, needs to be deleted from global table as well
-- but this does seem to work fine
testEntity = nil
entities[1] = nil


--testEntity2 = Entity.create()
--testEntity2:move(100, 220) -- testEntity:move() same as Entity.move(testEntity)
--testEntity2:scale(2, 2)
--testEntity:addSprite("slime", "assets/sprite/slime.png", 32, 32)

entityTable = {}
for i = 1, 100 do
	entityTable[i] = create()
	entityTable[i]:move(100 + i * 100, 220 + i * 200)
	entityTable[i]:scale(1 * (i % 10), 1 * (i % 10))
	entityTable[i]:addSprite("player", "assets/sprite/player.png", 48, 48)
	entityTable[i]:addAnimation("default", 1, 6)
	entityTable[i]:setAnimation("default")
	entityTable[i]:addCollider("enemy", 15, 35)
	entityTable[i]:addPhysics(2.0, 0.3)
end
