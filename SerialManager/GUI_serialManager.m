classdef GUI_serialManager < handle
    %Builds uicontrol objects for the 300W Controller GUI. Sets default values and assigns
    %specific values as defined in GUI_Builder.
    %
    %Stores all uicontrol handles in the handles property
    
    properties       
        serObj
        baudRate = 115200;
        port
        timeout = 5;%(seconds) how long to wait for a send/receive serial command 
        %before we say it didn't work and the error function is called
        timePeriod = 5;%(seconds) how often we check connects
        ctrlObj
    end
    
    methods
        
        function [SerialObj] = GUI_serialManager(SerialObj,CtrlObj)
            SerialObj.ctrlObj = CtrlObj;
            
        end
        
%         function CallObj = callBack(CallObj,handle,event,CtrlObj,BldObj)
%             
%             switch handle.Tag
%                 case 'reset_btn' % for the Reset Button
%                     serialObjs = instrfind('Port','COM3');
%                     if ~isempty(serialObjs)                       
%                         for kk = 1:length(serialObjs)
%                             if ~strcmp(serialObjs(kk).Status,'open')
%                                 delete(serialObjs(kk))
%                             else
%                                 fclose(serialObjs);
%                                 delete(serialObjs(kk))
%                             end
%                         end
%                         serialObjs = instrfind('Port','COM3');
%                         if isempty(serialObjs)
%                             serialObjs = serial('COM3','BaudRate',115200);
%                             fopen(serialObjs);
%                         end
%                     else
%                         serialObjs = serial('COM3','BaudRate',115200);
%                         fopen(serialObjs);
%                     end
%                     serialObjs.Terminator = 'CR';
%                     fprintf(serialObjs,'Hello World!!');
%                     idn = fscanf(serialObjs);
%                     disp(idn)
%                    
%                     1;
%                 case 'key_btn' % for the 'Laser Key' button
%                 case 'rStart_btn'% for the 'Enable Remote Start' button
%                     
%                 case 'prgmRun_rbtn' % for the 'Programmed Run' radial button
%                     handle.Value = 1;
%                     CtrlObj.handles.manualRun_rbtn.Value = 0;
%                     CallObj.panEnableCtrl('prgmRun_rbtn',CtrlObj);
%                     CallObj.simplePrgmChange(CtrlObj);
%                 case 'manualRun_rbtn' %for the 'Manual Run' radial button
%                     handle.Value = 1;
%                     CtrlObj.handles.prgmRun_rbtn.Value = 0;
%                     CallObj.panEnableCtrl('manualRun_rbtn',CtrlObj);
%                     CallObj.manualChange(CtrlObj,'manCurrent')
%                 case 'emission_btn' % for the 'Turn Emission On' button
%                     
%                 case {'min','max','delay','onDur','offDur','riseT','fallT','phase'} %for the Simple programmed area
%                     CallObj.simplePrgmChange(CtrlObj);
%                 case {'endInput','endPopup'} %For the end condition area
%                     if CtrlObj.panHandles.waveformPans.Selection == 1
%                         %Simple panel tab is selected
%                         CallObj.simplePrgmChange(CtrlObj)
%                     elseif CtrlObj.panHandles.waveformPans.Selection == 2
%                         %Complex panel tab is selected
%                     end
%                     switch CtrlObj.handles.endPopup.String{CtrlObj.handles.endPopup.Value}                        
%                         case 'Cycles (#)'
%                             CtrlObj.handles.endInput.Enable = 'on';
%                         case 'Run Time (sec)'
%                             CtrlObj.handles.endInput.Enable = 'on';
%                         case 'User Stop'
%                             CtrlObj.handles.endInput.Enable = 'off';
%                     end
%                 case {'manPower','manCurrent'}                    
%                     CallObj.manualChange(CtrlObj,handle.Tag)
%             end   
%             
%         end

        function [SerialObj,serialList] = availablePorts(SerialObj,obj,event,varargin)            
            serialList = seriallist;
            [handle] = SerialObj.findChild(obj,'Label','Serial');
            delete(handle.Children);%get rid of old children
            for kk= 1:length(serialList) %get new children             
                uimenu(handle,'Label',char(serialList(kk)),'Callback',@SerialObj.comSelection)           
            end            
        end
        
        function [SerialObj] = comSelection(SerialObj,obj,~)
            SerialObj.port = obj.Label;
            SerialObj = SerialObj.createSerialPort();
        end
        
        function [handle] = findChild(SerialObj,Parent,Field,Value)            
            names = fieldnames(Parent);
            if ismember(Field,names) &&  (ischar(Value) && ...
                    strcmp(Parent.(Field),Value)) || (isnumeric(Value) && Parent.(Field) == Value)
                handle = Parent;                
            else
                if ~isempty(Parent.Children)
                    for kk = 1:length(Parent.Children)
                        [handle] = findChild(SerialObj,Parent.Children(kk),Field,Value);  
                        if ~isempty(handle)
                            break
                        end
                    end
                else
                    handle = [];                    
                end
            end
        end
        
        function SerialObj = createSerialPort(SerialObj)
            
            %check to see if port has already been setup
            serObjs = instrfind('Port',SerialObj.port);
            if ~isempty(serObjs)
%                 if length(serialObjs)>1
                    for kk = 1:length(serObjs)
                        if ~strcmp(serObjs(kk).Status,'open')
                            delete(serObjs(kk))
                        else
                            try
                                %test connection here
                                %if it works update laser connected light
                                [SerialObj] = SerialObj.checkConnection(SerialObj.serObj, []);
                            catch                                
                                fclose(serObjs);
                                delete(serObjs(kk))
                            end
                        end
                    end
                serObjs = instrfind('Port',SerialObj.port);
                if isempty(serObjs)                    
                    [SerialObj,serObjs] = setSerialProps(SerialObj);
                end
            else                
                [SerialObj,serObjs] = setSerialProps(SerialObj);
            end
            SerialObj.serObj = serObjs;
            1;
            function [SerialObj,serObjs] = setSerialProps(SerialObj)
                serObjs = serial(SerialObj.port);
                serObjs.Terminator = 'CR';
                serObjs.TimerPeriod = SerialObj.timePeriod;%check connection every x seconds
                serObjs.TimerFcn = @SerialObj.checkConnection;
                serObjs.BaudRate = SerialObj.baudRate;
                serObjs.BytesAvailableFcnMode = 'terminator';
                serObjs.BytesAvailableFcn = @SerialObj.parseBytes;
                serObjs.Timeout = SerialObj.timeout;
                serObjs.ErrorFcn = @SerialObj.Error;
    %             fprintf(serialObjs,'Hello World!!');
%                 SerialObj.serialObj = serialObjs;
                fopen(serObjs);
                    % check connection here
            end
            
        end
        
        function [SerialObj] = checkConnection(SerialObj,obj, event)
            1;
            if strcmp(obj.TransferStatus,'idle')
                %ping the laser to see if it is connected
                 fprintf(obj,'ping');
                 [SerialObj,String] = SerialObj.parseBytes(obj,[]);
                 switch upper(String)
                     case 'YES'%Galileo and laser are connected 
                         SerialObj.handles
                     case 'NO' %Galileo is connected but laser isn't
                         
                     case '' % Nothing is connected
                         
                 end
                
            else %assume connection is still good because data is being transfered
                
                
            end
            
            
        end
        
        function  [SerialObj,String,Numeric] = parseBytes(SerialObj,obj,event)
             idn = fscanf(obj);
             idn(idn == ' ') = []; %remove spaces
             disp(idn)
             aa = 0;% bb = 0;
             colChk = false;
             for kk = 1:length(idn)
                 if ~isnan(num2double(idn(kk)))% true then is supposed to be a string
                     aa = aa+1;
                     String(aa) = ind(kk);                     
                     colChk =strcmp(String(aa),':'); 
                 else %it is supposed to be a number
                     if colChk                         
%                      bb = bb+1;
                        Numeric = num2double(idn(kk:end));
                        if isnan(Numeric)
                            aa = aa+1;
                            String(aa:length(ind(kk:end))) = ind(kk:end); %#ok<COLND>
                        end
                        break
                     end
                 end    
             end
             colonInd = strfind(String,':');
             if ~isempty(colonInd)
                designator = String(1:colonInd-1);
             else
                designator = String; 
             end
             
             switch designator %handles actions that always happen for a given laser response
                 case 'SDC' %This is laser reponse to setting diode current
                     
                     
                 case 'RCS' %Laser response to 'read current setpoint'
                     %update live graph if laser is being operated
                 case 'RNC'
                     %reponse to 
                 case 'RDC'
                 case 'ROP'
                 case 'RFV'
                 case 'RCT'
                 case 'STA'
                 case 'EMON'
                 case 'EMOFF'
                 case 'EMOD'
                 case 'DMOD'
                 case 'EEC'
                 case 'DEC'
                 case 'RERR'
                 case 'ABN'
                 case 'ABF'
                 case 'EEABC'
                 case 'DEABC'
                 case 'SFWS'
                 case 'RFWS'
                 case 'RMEC'
             end
                    
        end
        
        function [SerialObj] = Error(SerialObj,obj, event)
            %Time out occured, means the laser and the Galileo are not
            %connected
            disp('Timeout')
            
            
        end
        
    end
end
      