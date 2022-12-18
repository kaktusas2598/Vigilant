-- TODO: need to import global entity table and some helper methods to all states to make scripting easier
background = Entity.create()
background:addSprite("clouds2", "", 800, 600)
background:addBackground(1)

title = Entity.create()
title:addLabel(0, 10, "VIGILANT ENGINE", "arcade", 0, 102, 204, 255)
title:setLabelAlignment(1, 0)

-- FIXME: Currently listener will be triggered, but thisId is useless,
-- because I couldn't get entities table be shared across multiple scripts
function onPlayClick(thisId)
    playSound("button")
    -- TODO: make this binding better, accept state id or ScreenState enum
    changeState()
end

function onQuit(thisId)
    quit()
end

playButton = Entity:create()
playButton:move((getScreenWidth() - 300)/2, (getScreenHeight() - 175)/2)
playButton:addSprite("playbutton", "", 300, 100)
playButton:setAbsolutePosition(true)
playButton:addButton()
playButton:setButtonListener("onPlayClick")

quitButton = Entity:create()
quitButton:move((getScreenWidth() - 300)/2, (getScreenHeight() + 75)/2)
quitButton:addSprite("exitbutton", "", 300, 100)
quitButton:setAbsolutePosition(true)
quitButton:addButton()
quitButton:setButtonListener("onQuit")

