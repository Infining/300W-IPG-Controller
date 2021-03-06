classdef LED < handle
    
    properties (SetObservable)
        %LED controls
        
        BackgroundColor = 'none';%color surrounding the LED: Inputs: 'none', or normal color inputs ('k'or [0 0 0])
        BorderColor = [.5 .5 .5]; %Edge color of LED
        BorderWidth = 1; %thickness of LED border (LineWidth)
        Callback = '';
        Children
        Color = 'y'; % Status color of the LED
        Enable = 'true';
        HorizontalAlignment = 'center';% 'left', 'center', 'right'
        Marker = 'o'; %Shape of the LED - same as plot shapes see plot docs
%         MarkerEdgeColor = [.5 .5 .5]; %Edge color of LED
        MarkerSize = 15; %Size of the LED
        Parent %parent object
        Position = [0,0];% x y plot position
        
        
        %Controlled via the panel handle
        
                
        Tag = '';
        Type = 'LED';
        UserData
        VerticalAlignment = 'middle'; % 'top', 'middle','bottom'
        Visible = 'on'; %'on' or 'off'
        
        
        
    end
    properties
       
    end
    
    
    methods
        function LEDobj = LED(Parent)
            LEDobj.attachListener;
            Pan = uipanel('Parent',Parent,'BackgroundColor',Parent.BackgroundColor);
            Pan.BorderWidth = 0;
            Axes = axes(Pan); 
            Plot = plot(Axes,0,0,'o');
            
            Axes.XLim = [-1,1];
            Axes.YLim = [-1,1];
            Axes.Visible = 'off';
            Axes.ActivePositionProperty = 'Position';
            
            LEDobj.Children.Panel = Pan;
            LEDobj.Children.Axes = Axes;
            LEDobj.Children.Plot = Plot;
            LEDobj.Parent = Parent;
            Plot.Color = LEDobj.BorderColor;
            Plot.LineWidth = LEDobj.BorderWidth;
            Plot.MarkerFaceColor = LEDobj.Color;   
            Plot.Marker = LEDobj.Marker;
            Plot.MarkerSize = LEDobj.MarkerSize;
        end
        
        function attachListener(LEDobj)
            names = fieldnames(LEDobj);
            names(strcmp(names,'Children')) = '';
            addlistener(LEDobj, names,'PostSet',@LED.LED_Manager);
        end
    end
    methods (Static)
        function LED_Manager(metaProp,eventData,varargin)
            LEDobj = eventData.AffectedObject;
            changeProp = metaProp.Name;
            Pan = LEDobj.Children.Panel;
%             Axes = LEDobj.Children.Axes;
            Plot = LEDobj.Children.Plot;
            Plot.Marker = LEDobj.Marker;
            switch changeProp
                case 'BackgroundColor'
                    if strcmp(LEDobj.BackgroundColor,'none')
                        Pan.BackgroundColor = LEDobj.Parent.BackgroundColor;
                    else
                        Pan.BackgroundColor = LEDobj.BackgroundColor;
                    end                    
                case 'BorderColor'
                    Plot.Color = LEDobj.BorderColor;
                case 'BorderWidth'
                    Plot.LineWidth = LEDobj.BorderWidth;
                    
                case 'Callback'
                case 'Color'
                    Plot.MarkerFaceColor = LEDobj.Color;
                case 'Enable'
                    
                case 'HorizontalAlignment'
                case 'Marker'
                    Plot.Marker = LEDobj.Marker;
                case 'MarkerSize'
                    Plot.MarkerSize = LEDobj.MarkerSize;
                case 'Position'
                case 'Type'
                    LEDobj.Type = 'LED';%can't change type
                case 'VeritcalAlignment'
                    
                case 'Visible'
                    Pan.Visible =  LEDobj.Visible;
            end
            
            
            
            1;
        end
        
        
    end
end