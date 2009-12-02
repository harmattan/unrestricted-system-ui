require 'test/unit'
require 'matti'
require 'dbus'
include MattiVerify


class PINQueryTest < Test::Unit::TestCase

  def initialize (args)
    super(args)
puts "Initialize once before all tests.."
     # set the sut used
     @sut = MATTI.sut(:Id => 'sut_qt_maemo')
    # creating the Dbus service and mapping it to the path /com/nokia/csd/sim
#    bus = DBus.session_bus
#    service = bus.request_service("com.nokia.csd.SIM")
#    @server = DBusServer.new("/com/nokia/csd/sim")
#    service.export(@server)
#    loop = DBus::Main.new
#    loop << bus
#    @dbus_thread=Thread.new { loop.run } 
#
  end


  def setup 
    @app=@sut.run(:name => 'sysuid')
puts "Setup before each test.."
    # dbus call to wake up sysuid application..
    system("dbus-send --system --print-reply --dest=com.nokia.systemui.pin /com/nokia/systemui/pin com.nokia.systemui.pin.PinCodeQuery.LaunchPinQuery string:'PIN'")
sleep 1
  end

  def teardown 
    begin
puts "Teardown after each test.."
      @app.close
#      Thread.kill(@dbus_thread)
sleep 1
    end 
  end

  def test_1_launch_PIN_code_query
puts "testing Launch pin query case.."
    # verify that Pin code query is visible
puts " step 1: verify that pin query is visible.."
sleep 1
    verify {@app.test_object_exists?( "Button", :text => "Enter" ) == 'true'}

if @app.DuiTextEdit( :name => 'codeEntry' ).attribute("text").length == 3
 puts "Mitä hittoa.."
end

mikas= @app.DuiTextEdit( :name => 'codeEntry' ).attribute("text").length
puts "Tähän pitäs tulla noin nolla..: '#mikas'"
    verify {@app.DuiTextEdit( :name => 'codeEntry' ).attribute("text").length == 3}

    # press Cancel and verify that pin query is not visble anymore by verifying that the Enter button is not visible 
puts " step 2: press cancel and verify that the Enter button is not visible anymore.."
sleep 1
    verify {@app.DuiButton( :name => 'cancelButton' ).tap}
    verify {@app.test_object_exists?( "Button", :text => "Enter" ) == 'false'}

    # do the dbus call to wake up sysuid application again
puts " step 3: start pin query again.."
sleep 1
    system("dbus-send --system --print-reply --dest=com.nokia.systemui.pin /com/nokia/systemui/pin com.nokia.systemui.pin.PinCodeQuery.LaunchPinQuery string:'PIN'")
    # verify that Pin code query is visible
    verify {@app.DuiTextEdit( :name => 'codeEntry' ).attribute("text").length == 8}
  end

  def test_2_enter_correct_code
puts "testing Enter correct pin code case.."
    # enter 3 digits to pin code
puts " step 1: enter 3 digits and verify that Enter is not enabled.."
sleep 1
    for i in 1..3 
      # tap some digit 3 times
       verify {@app.DuiButton( :name => 'numpadButton0' ).tap}
    end
    # verify that the entered code is 3 digits long
    verify {@app.DuiTextEdit( :name => 'codeEntry' ).attribute("text").length == 3}
    # verify that the enter button is not enabled when only 3 digits entered
    verify {@app.DuiButton( :name => 'enterButton' ).attribute("enabled") =='false'}

puts " step 2: try to add 7 digits and verify that only 8 digits can be entered.."
sleep 1
    # .. 7 digits more
    for i in 1..7 
       verify {@app.DuiButton( :name => 'numpadButton1' ).tap}
    end
    # verify that the entered code is only 8 digits long
    verify {@app.DuiTextEdit( :name => 'codeEntry' ).attribute("text").length == 8}

puts " step 3: erase entered digits by pressing Backspace 8 times and verify that code field is empty.."
sleep 1
    # clear digits by hitting the backspace button until no more digits left
    while @app.DuiTextEdit( :name => 'codeEntry' ).attribute("text").length > 0
      verify {@app.DuiButton( :name => 'backspaceButton' ).tap}
    end
    # verify that the entered code is 0 digits long
    verify {@app.DuiTextEdit( :name => 'codeEntry' ).attribute("text").length == 0}

puts " step 4: enter correct pin code, press enter and then verify that the Enter button is not visible anymore"
sleep 1
    # enter correct pin code
    verify {@app.DuiButton( :name => 'numpadButton4' ).tap}
    verify {@app.DuiButton( :name => 'numpadButton3' ).tap}
    verify {@app.DuiButton( :name => 'numpadButton2' ).tap}
    verify {@app.DuiButton( :name => 'numpadButton1' ).tap}
    # verify that the entered code is 4 digits long
    verify {@app.DuiTextEdit( :name => 'codeEntry' ).attribute("text").length == 4}
    # verify that the enter button is not enabled when only 3 digits entered
    verify {@app.DuiButton( :name => 'enterButton' ).attribute("enabled") =='true' }

    # press enter
    verify {@app.DuiButton( :name => 'enterButton' ).tap}

    # TODO, REPLACE THIS WITH: verify that Dbus server returns 'ok'
    verify {@app.test_object_exists?( "Button", :text => "Enter" ) == 'false'}
  end

end

