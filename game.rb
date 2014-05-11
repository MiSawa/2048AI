#!/usr/bin/env ruby
#coding: utf-8

require 'selenium-webdriver'

$URL = 'http://gabrielecirulli.github.io/2048/'
$Program = ARGV[0]

class Game
  def initialize
    @driver = Selenium::WebDriver.for :firefox
    @driver.navigate.to $URL
    @driver.execute_script('game = new GameManager(4, KeyboardInputManager, HTMLActuator, LocalStorageManager);')
    @driver.execute_script('game.restart();')
  end
  def cells
    @driver.execute_script('return game.grid.cells').map{|r|
      r.map{|p| p.nil? ? 0 : p["value"] }
    }.transpose
  end
  def score
    @driver.execute_script('return game.score')
  end
  def move(dir)
    # 0: up, 1: right, 2: down, 3: left
    hoge = { :UP => 0, :RIGHT => 1, :DOWN => 2, :LEFT => 3 }
    dirnum = hoge[dir.upcase]
    raise 'no such dir' unless dirnum
    @driver.execute_script("game.move(#{dirnum});")
    if @driver.execute_script('return game.won && !game.keepPlaying;')
      @driver.execute_script('game.keepPlaying = true; game.actuator.continueGame();')
    end
  end
  def terminated?
    @driver.execute_script('return game.isGameTerminated();')
  end
  def to_s
    cells.map{|r| r * " "} * "\n"
  end
end

class BotPlayer
  def initialize(program)
    r, @bot_in = IO.pipe
    @bot_out, w = IO.pipe
    pid = spawn(program, :in => r, :out => w, :err => :out)
    at_exit do
      self.puts 0
      @bot_in.close
      w.close
    end
  end
  def puts s
    @bot_in.puts s
  end
  def gets
    @bot_out.gets
  end
  def succ(game)
    self.puts 1
    self.puts game.score
    self.puts game
    game.move self.gets.chomp.to_sym
  end
end

bot = BotPlayer.new $Program
game = Game.new
until game.terminated?
  bot.succ game
end

