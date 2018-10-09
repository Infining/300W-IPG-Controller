% 300 W IPG Laser Controller (Laser Controller (300W IPG))
% This is the main script that launches and initializes the GUI that allows
% remote control of the 300 W IPG Laser

% tic
% try
%     guiObj = GUI_Main();
% % catch
% tic
    Path = fileparts(cd);
    addpath(genpath(Path))
% toc
% tic
    guiObj = GUI_Main();
% end
% toc