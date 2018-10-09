classdef GUI_Ctrl <handle
    %Builds uicontrol objects for the 300W Controller GUI. Sets default values and assigns
    %specific values as defined in GUI_Builder.
    %
    %Stores all uicontrol handles in the handles property
    
    
    properties (SetAccess = immutable)
        dflt
        dflt_unictrl
        dflt_txtbox
        dflt_all
        dflt_LED
        
    end
    properties  (SetAccess = public)
        handles
        panHandles
        font = 'Times New Roman';
    end
    properties (Constant = true)
        uiStyles = {'checkbox','edit','frame','listbox','popup','pushbutton',...
            'radiobutton','slider','text','togglebutton'};
    end
    methods
        
        function obj = GUI_Ctrl(varargin)
            [obj] = globalSettings(obj, varargin);
            
            obj.dflt.BackgroundColor = [0.9400 0.9400 0.9400];
            obj.dflt.FontAngle = 'normal';
            obj.dflt.FontName = obj.font;
            obj.dflt.FontSize = 12;
            obj.dflt.FontUnits = 'points';
            obj.dflt.FontWeight = 'normal';
            
            obj.dflt.HorizontalAlignment = 'center';%'left','center','right'
            obj.dflt.Visible = 'on';
            
            %Defaults for UIControl objects
            obj.dflt_unictrl = obj.dflt;
            obj.dflt_unictrl.Enable = 'on';%'on','off','inactive'
            obj.dflt_unictrl.Style = 'pushbutton';
            obj.dflt_unictrl.TooltipString = {' '};%allows for tool tips to appear when user scrolls on things
            obj.dflt_unictrl.Value = 1;
            obj.dflt_unictrl.ForegroundColor = 'k';
            
            %Defaults for text box objects
            obj.dflt_txtbox =obj.dflt;
            obj.dflt_txtbox.Interpreter = 'tex';
            obj.dflt_txtbox.LineStyle = 'none';
            obj.dflt_txtbox.LineWidth = 0.5;
            obj.dflt_txtbox.EdgeColor = 'none';
            obj.dflt_txtbox.FaceAlpha = 1;
            obj.dflt_txtbox.FitBoxToText = 'on';
            obj.dflt_txtbox.Margin = 0;
            obj.dflt_txtbox.VerticalAlignment = 'middle';
            obj.dflt_txtbox.Color = 'k';
            
            %Defaults for LEDs (status circles - axes in panel)
            %lets you build LED like status lights and control their
            %apperance
            
            %LED Defaults
            
            obj.dflt_LED.BackgroundColor = 'none';%color surrounding the LED: Inputs: 'none', or normal color inputs ('k'or [0 0 0])
            obj.dflt_LED.BorderColor = [.5 .5 .5]; %Edge color of LED
            obj.dflt_LED.BorderWidth = 1; %thickness of LED border (LineWidth)
            obj.dflt_LED.Color = 'y'; % Status color of the LED
            obj.dflt_LED.HorizontalAlignment = 'center';% 'left', 'center', 'right'
            obj.dflt_LED.Marker = 'o'; %Shape of the LED - same as plot shapes see plot docs
            obj.dflt_LED.MarkerSize = 15; %Size of the LED
            obj.dflt_LED.Position = [0,0];% x y plot position
            obj.dflt_LED.VerticalAlignment = 'middle'; % 'top', 'middle','bottom'
            obj.dflt_LED.Visible = 'on'; %'on' or 'off'
            
            obj.dflt_all =  mergeStruct(obj.dflt_txtbox,obj.dflt_unictrl);            
            
            function [struct1] = mergeStruct(struct1,struct2)
                names = fieldnames(struct2);
                for kk=1:length(names)
                    struct1.(names{kk}) = struct2.(names{kk});
                end
            end
            
        end
               
        
        function obj = build_uicontrols(obj,Parent,String, varargin)
            %Parent - parent object of status lights (boxes) at top of GUI
            %String - is a cell of strings of the names the lights/buttons will have
            %varargin - is a cell of name-value pairs for font properties i.e.:
            % 'FontName','Times New Roman'
            % 'FontSize', 12
            % 'ForegroundColor', 'r' or [1,0,0]
            %For full list of settable properties see the contstuctor
            %function above (GUI_Control)
            if any(strcmp('Style',varargin))
                ind = find(strcmp('Style',varargin));
                styles = varargin{ind+1};
            else
                styles = [];
            end
            if ~iscell(String)
                String  = {String};
            end
            txtInd = [];LEDInd  = [];uiInd = [];
            [tags,strings,styles] = tagParser(obj,String,styles);
            for kk = 1:length(String)
                if ismember({'text'},styles{kk})%
                    obj.handles.(tags{kk}) = annotation(Parent,'textbox','String',strings{kk});%
                    txtInd = [txtInd,kk]; %#ok<AGROW>
                elseif ismember({'LED'},styles{kk})
                    obj.handles.(tags{kk}) = LED(Parent);%
                    LEDInd = [LEDInd,kk]; %#ok<AGROW>
                elseif ismember(styles{kk},obj.uiStyles)
                    obj.handles.(tags{kk}) = uicontrol('Parent',Parent,'String',strings{kk});
                    uiInd = [uiInd,kk]; %#ok<AGROW>
                end
            end
            
            [properties] = bldPropertyTables(obj,obj.dflt_txtbox,tags,varargin,txtInd);
            obj.setProperties(tags(txtInd),obj.dflt_txtbox,properties);
                       
            [properties] = bldPropertyTables(obj,obj.dflt_LED,tags,varargin,LEDInd);
            obj.setProperties(tags(LEDInd),obj.dflt_LED,properties,'LED');
            
            [properties] = bldPropertyTables(obj,obj.dflt_unictrl,tags,varargin,uiInd);
            obj.setProperties(tags(uiInd),obj.dflt_unictrl,properties);
            
        end
       
        function [properties] = bldPropertyTables(obj,dflt,tags,varg,ind)
            if ~isempty(ind)
                tags = tags(ind);
                properties = struct2table(dflt);
                for kk = 2:length(tags)
                    properties(kk,:) = properties(1,:);
                end
                properties.Properties.RowNames = tags;
                
                if ~isempty(varg)
                    names = fieldnames(dflt);
                    for aa = 1:length(varg)
                        if ischar(varg{aa}) && ismember(varg{aa},names)
                            if isscalar(varg{aa+1})
                                value = repmat(varg{aa+1},[length(varg{aa+1}),length(tags)])';
                            elseif ischar(varg{aa+1})
                                value = repmat(varg(aa+1),[1,length(tags)])';
                            else
                                value = varg{aa+1}';
                                value = value(ind);
                            end
                            properties.(varg{aa}) = value;
                        end
                    end
                end
                
                if ismember('TooltipString',fieldnames(properties))
                    for kk = 1:length(properties.TooltipString)
                        if strcmp(properties.TooltipString{kk},' ')
                            properties.TooltipString{kk} = '';
                        end
                    end
                end
            else
                properties = [];
            end
            
        end
        
        function obj=setProperties(obj,tags,dflt,properties, varargin)
            if ~isempty(tags)
                if isstring(tags)
                    tags = {tags};
                end
                names = fieldnames(dflt);
                
                for kk = 1:length(tags)
                    
                    for aa = 1:length(names)
                        %                     try
                        if iscell(properties.(names{aa})(kk,:))
                            obj.handles.(tags{kk}).(names{aa})= properties{tags(kk),names{aa}}{1};
%                             if ismember({'TooltipString'},names{aa}) && ~isempty(properties{tags(kk),names{aa}}{1})%use java properties to make it so tool tip shows up when button is disabled
%                                 jobj = findjobj(obj.handles.(tags{kk}));
%                                 set(jobj,'ToolTipText',properties{tags(kk),names{aa}}{1});                                
%                             end                                
                        elseif isstruct(properties.(names{aa})(kk,:))
                            1;
                        else
                            obj.handles.(tags{kk}).(names{aa})= properties{tags(kk),names{aa}};
%                             if ismember({'TooltipString'},names{aa}) && ~isempty(properties{tags(kk),names{aa}}) %use java properties to make it so tool tip shows up when button is disabled
%                                 jobj = findjobj(obj.handles.(tags{kk}));
%                                 set(jobj,'ToolTipText',properties{tags(kk),names{aa}});                                
%                             end  
                        end
                        %                     catch
                        %                         1;
                        %                     end
                        obj.handles.(tags{kk}).Tag = (tags{kk});
                    end
                end
            end
        end
        
        function [tags,strings,style] = tagParser(obj,strings,style)
            if isstring(strings)
                strings = {strings};
            end
            if ~iscell(style)&&~isempty(style)
                style = {style};
            end
            %first determine if a popup menu is involved
            %If there is we will have to go in and pull out the nested cell
            % With or w/o a popup involved there will only be as many tags
            % as there are top lvl elements in strings
            
            if ~isempty(style)&& ismember('popup',style)%if true popup menu is involved
                indsLog = strcmp(style,'popup');
                string4Tag(~indsLog) = strings(~indsLog);
                inds = find(indsLog);
                for qq = 1:length(inds)
                    string4Tag{inds(qq)} = strings{inds(qq)}{1};
                end
                [tags,stringsTemp] = obj.removeColon(string4Tag);
                strings(~indsLog) = stringsTemp(~indsLog);
                for qq = 1:length(inds)
                    strings{inds(qq)}{1} = stringsTemp{inds(qq)};
                end
            else
                [tags,strings] = obj.removeColon(strings);
            end
            if isempty(style)
                style = {'pushbutton'};
                style = repmat(style,length(strings),1);
            elseif isscalar(style)
                style = repmat(style,length(strings),1);
            end
            
        end
        
        function [tags,strings] = removeColon(obj,strings)
            tags = cell(1,length(strings));
            for kk = 1:length(strings)
                inds = strfind(strings{kk},'::');
                if ~isempty(inds)
                    tags{kk} = strings{kk}(1:inds(1)-1);
                    strings{kk} = strings{kk}(inds(1)+2:end);
                else
                    strings{kk} = strings{kk};
                    tags{kk} = matlab.lang.makeValidName(strings{kk});
                end
            end
        end
        
        
    end
    
end