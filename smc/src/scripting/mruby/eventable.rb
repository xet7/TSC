# The Eventable mixin makes an object able to receive events.
module ::Eventable

  # Takes a block and registers it for execution when the
  # event with name +evt_name+ occurs.
  def bind(evt_name, &callback)
    @callbacks ||= Hash.new{|h, k| h[k] = []}
    @callbacks[evt_name].push(callback)
  end

  # Executes all callbacks for the event +evt_name+, and
  # passes all further arguments to all the registered
  # callbacks. The first argument passed to the block
  # however is always +evt_name+.
  def execute(evt_name, *args)
    @callbacks[evt_name].each{|cb| cb.call(evt_name, *args)}
  end

end
