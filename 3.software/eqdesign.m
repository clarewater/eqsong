Fs = 44100;

% 中心频率和带宽（Hz）
freqs = [100, 400, 3000, 10000];
bws   = [ 50, 200, 2000, 3000];

% 归一化
centerFreqs = freqs / (Fs/2);
bandwidths  = bws / (Fs/2);

% 固定滤波器阶数
N = 2;

% 所有要测试的增益（单位：dB）
gains = [-6, -3, 0, 1, 2, 3, 6];

% 遍历每个频点
for i = 1:length(freqs)
    name = sprintf("f%d_sos", freqs(i));
    fprintf("float %s[35] = {\n", name);

    row = [];
    for g = gains
        [B, A] = designParamEQ(N, g, centerFreqs(i), bandwidths(i), "Orientation","row");
        % 合成 sos 行 [b0, b1, b2, -a1, -a2]
        sos = [B, -A(:,2:3)];

        % 添加到输出队列
        row = [row, sos];
    end

    % 按每行 5 个元素输出
    for k = 1:length(row)
        fprintf("    %.8ff", row(k));
        if k ~= length(row)
            fprintf(", ");
        end
        if mod(k, 5) == 0
            fprintf("\n");
        end
    end

    fprintf("};\n\n");
end
[B, A] = designParamEQ(N, [6,-2,-2,6], centerFreqs, bandwidths, "Orientation","row");
freqz(B,A);
