function read_bf_socket()

while 1
    port = 8090;
    t = tcpip('0.0.0.0', port, 'NetworkRole', 'server');
    t.InputBufferSize = 1024;
    t.Timeout = 100;
    fprintf('Waiting for connection on port %d\n',port);
    fopen(t);
    fprintf('Accept connection from %s\n',t.RemoteHost);

    clf=figure(1);
    set(clf,'position',[0,0,1000,500])
    t1 = 0;
    y=0;
    y1 = 0;
    y2 = 0;
    y3 = 0;
    temp = 0;
    temp1 = 0;
    temp2 = 0;
    temp3 = 0;
    
    xlabel('Subcarrier index');
    ylabel('SNR (dB)');

    csi_entry = [];
    index = -1; 
    broken_perm = 0;
    triangle = [1 3 6];
    
    while 1
        s = warning('error', 'instrument:fread:unsuccessfulRead');
        try
            field_len = fread(t, 1, 'uint16');
        catch
            warning(s);
            disp('Timeout, please restart the client and connect again.');
            break;
        end
        code = fread(t,1);

        if (code == 187) 
            bytes = fread(t, field_len-1, 'uint8');
            bytes = uint8(bytes);
            if (length(bytes) ~= field_len-1)
                fclose(t);
                return;
            end
        else if field_len <= t.InputBufferSize
            fread(t, field_len-1, 'uint8');
            continue;
            else
                continue;
            end
        end
        if (code == 187) 
            csi_entry = read_bfee(bytes);
            perm = csi_entry.perm;
            Nrx = csi_entry.Nrx;
            if Nrx > 1
                if sum(perm) ~= triangle(Nrx) 
                    if broken_perm == 0
                        broken_perm = 1;
                    end
                else
                    csi_entry.csi(:,perm(1:Nrx),:) = csi_entry.csi(:,1:Nrx,:);
                end
            end
        end
        
        csi = get_scaled_csi(csi_entry);
        tmp = ifft(csi,60);
        f_tmp = fft(tmp,50);
        [b,a] = butter(6,0.2*pi);
        b_tmp = filter(b,a,f_tmp);
        
        test = db(abs(squeeze(csi(1,1,:)).'));
        t_tmp = db(abs(squeeze(tmp(1,1,:)).'));
        f_tmp = db(abs(squeeze(f_tmp(1,1,:)).'));
        b_tmp = db(abs(squeeze(b_tmp(1,1,:)).'));
        
        t1 = t1+1;
        
        y = [y test(1,1)-temp];
        temp = test(1,1);
        
        y1 = [y1 t_tmp(1,1)-temp1];
        temp1 = t_tmp(1,1);
        
        y2 = [y2 f_tmp(1,1)-temp2];
        temp2 = f_tmp(1,1);
        
        y3 = [y3 b_tmp(1,1)-temp3];
        temp3 = b_tmp(1,1);
        
        subplot(3,1,1);
        plot(y);        
        axis([0+(t1/1.5) 300+(t1/1.5) 0 inf]);
        ylim([-25 25]);
        
        subplot(3,1,2);
        plot(y1);         
        axis([0+(t1/1.5) 300+(t1/1.5) 0 inf]);       
        ylim([-25 25]);
        
        subplot(3,1,3);
        plot(y3);         
        axis([0+(t1/1.5) 300+(t1/1.5) 0 inf]);       
        ylim([-25 25]);
        
        drawnow;
        csi_entry = [];
    end
    
    fclose(t);
    delete(t);
end

end
