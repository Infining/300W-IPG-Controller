function [obj] = globalSettings(obj, varargin)

if ~isempty(varargin)
    for kk = 1:length(varargin{1})
        if ischar(varargin{1}{kk})
            switch varargin{1}{kk}
                case 'Font'
                    obj.font = varargin{1}{kk+1};                    
            end
        end
    end
end
end