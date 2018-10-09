classdef GUI_Main    
    %Calls the functions from GUI_Builder to create the 300W Controller GUI
    %Assigns a generic Callback function for all uicontrol objects. This
    %function calls functions specific to the button pressed
    
    properties
        font = 'Times';
    end
    %
    methods
        %
        function obj = GUI_Main
            BldObj = GUI_Builder('Font',obj.font);
            CtrlObj = GUI_Ctrl('Font',obj.font);%create instance of class that builds buttons and handles
            %their functions
            CallObj = GUI_Callback();
            SerialObj = GUI_serialManager([],CtrlObj);
            
            % Create Main Figure & divide Main figure into tab panels
            
            [f,CtrlObj] = BldObj.mainFig(CtrlObj,SerialObj);
            
            %%%%% Create 1st tab inside of Main Figure: Main
            [BldObj,CtrlObj] = BldObj.Tabs(CtrlObj,CtrlObj.panHandles.masterPanels);%Main is setup to be divided into horizontal regions (each child will define a new column)
                     
            
            %Assign Callbacks:
            %general Callback function for all handles collected
            %and sort out what we want it to do depending on what was
            %clicked
            Names = fieldnames(CtrlObj.handles);
            for kk = 1: length(Names)
                try
                if ismember('Type',fieldnames(CtrlObj.handles.(Names{kk}))) && strcmp(CtrlObj.handles.(Names{kk}).Type,'uicontrol')
                    if ~strcmp(CtrlObj.handles.(Names{kk}).Style,'text')
                        CtrlObj.handles.(Names{kk}).Callback = {@CallObj.callBack,CtrlObj,BldObj};
                    end
                end      
%                 1;
                catch
                    1;
                end
            end
            
            CallObj.simplePrgmChange(CtrlObj);
            
            
            drawnow
            
%             while true
%               drawnow  
%             end
                
            
        end
        
    end
    
end