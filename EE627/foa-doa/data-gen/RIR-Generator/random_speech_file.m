function [path_sampled_file] = random_speech_file(data_folder_path)
%     files = dir('../../data/speech/*.flac');
    files = dir(strcat(data_folder_path, '/*.flac'));
    sampled_file = files(randi(length(files), 1, 1));
    path_sampled_file = strcat(sampled_file.folder, "/", sampled_file.name);
end