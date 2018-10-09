classdef PropLis < handle
   % Define a property that is SetObservable
   properties (SetObservable)
      ObservedProp = 1
   end
   methods
      function attachListener(obj)
         %Attach a listener to a PropListener object
         addlistener(obj,'ObservedProp','PostSet',@PropLis.propChange);
      end
   end
   methods (Static)
      function propChange(metaProp,eventData)
         % Callback for PostSet event
         % Inputs: meta.property object, event.PropertyEvent
         h = eventData.AffectedObject;
         propName = metaProp.Name;
         disp(['The ',propName,' property has changed.'])
         disp(['The new value is: ',num2str(h.ObservedProp)])
         disp(['Its default value is: ',num2str(metaProp.DefaultValue)])
      end
   end
end