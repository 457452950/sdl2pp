#pragma once
#ifndef SDL2PP_SDL2PP_SDL_SDLMIXER_HPP_
#define SDL2PP_SDL2PP_SDL_SDLMIXER_HPP_

#include <vector>
#include <memory>
#include <string>
#include <functional>

#include <SDL_audio.h>
#include <SDL_mixer.h>

namespace sdlpp {

class SDLMixer {
public:
    using FadingType = Mix_Fading;
    using MusicType  = Mix_MusicType;

    /**
     *
     * @param frequency
     * @param format
     * @param channels The audio device channels are generally 1 for mono output, or 2 for stereo, but the brave can try
     * surround sound configs with 4 (quad), 6 (5.1), 7 (6.1) or 8 (7.1).
     * @param chunk_size
     * @return
     */
    bool OpenAudio(int    frequency  = MIX_DEFAULT_FREQUENCY,
                   Uint16 format     = MIX_DEFAULT_FORMAT,
                   int    channels   = 1,
                   int    chunk_size = 2048);

    /**
     *
     * @param frequency
     * @param format
     * @param channels
     * @return true if the audio device has been opened.
     */
    bool QuerySpec(int *frequency, Uint16 *format, int *channels);


    const std::vector<std::string> &GetChunkDecoders() const;

    const std::vector<std::string> &GetMusicDecoders() const;

    bool HasChunkDecoder(std::string_view name) const;

    bool HasMusicDecoder(std::string_view name) const;

    static SDLMixer *GetInstance() {
        static SDLMixer instance_;
        return &instance_;
    }

    ~SDLMixer() {
        if(opened_)
            Mix_CloseAudio();
    }

private:
    SDLMixer() = default;
    bool opened_{false};

    std::vector<std::string> chunk_decoders_;
    std::vector<std::string> music_decoders_;
};

class MixChunk {
public:
    static std::shared_ptr<MixChunk> LoadWAV(std::string_view file) {
        return MixChunk::Create(Mix_LoadWAV(file.data()));
    }

    static std::shared_ptr<MixChunk> QuickLoad_WAV(uint8_t *mem) { return MixChunk::Create(Mix_QuickLoad_WAV(mem)); }

    static std::shared_ptr<MixChunk> QuickLoad_RAM(uint8_t *mem, uint32_t len) {
        return Create(Mix_QuickLoad_RAW(mem, len));
    }

    Mix_Chunk       *Get() { return chunk_; }
    const Mix_Chunk *Get() const { return chunk_; }

    bool Valid() const { return chunk_ != nullptr; }

    int Volume() const { return Mix_VolumeChunk(chunk_, -1); }

    void SetVolume(int volume) { Mix_VolumeChunk(chunk_, volume); }

    ~MixChunk() {
        if(chunk_)
            Mix_FreeChunk(chunk_);
    }

private:
    static std::shared_ptr<MixChunk> Create(Mix_Chunk *&&chunk) {
        return std::shared_ptr<MixChunk>(new MixChunk(std::move(chunk)));
    }

    explicit MixChunk(Mix_Chunk *&&chunk) : chunk_(chunk) {}

    Mix_Chunk *chunk_{nullptr};
};

class MixChannel {
public:
    MixChannel(int channel_id) : id_(channel_id) {}

    int GetId() const { return id_; }

    bool SetPanning(int left, int right) { return Mix_SetPanning(id_, left, right) != 0; }

    bool SetPosition(int16_t angle, uint8_t distance) { return Mix_SetPosition(id_, angle, distance) != 0; }

    bool SetDistance(uint8_t distance) { return Mix_SetDistance(id_, distance) != 0; }

    bool SetReverseStereo(bool flip) { return Mix_SetReverseStereo(id_, flip ? 1 : 0) != 0; }

    /**
     *
     * @param channel If a specific channel was requested,
     *                  and there is a chunk already playing there,
     *                  that chunk will be halted and the new chunk will take its place.
     * @param chunk
     * @param loops
     * @return
     */
    bool Play(std::shared_ptr<MixChunk> chunk, int loops) { return Mix_PlayChannel(id_, chunk->Get(), loops) != -1; }
    bool Play(std::shared_ptr<MixChunk> chunk, int loops, int ticks) {
        return Mix_PlayChannelTimed(id_, chunk->Get(), loops, ticks) != -1;
    }

    bool FadeIn(std::shared_ptr<MixChunk> chunk, int loops, int ms) {
        return Mix_FadeInChannel(id_, chunk->Get(), loops, ms) != -1;
    }

    bool FadeInTimed(std::shared_ptr<MixChunk> chunk, int loops, int ms, int ticks) {
        return Mix_FadeInChannelTimed(id_, chunk->Get(), loops, ms, ticks) != -1;
    }

    int GetVolume() { return Mix_Volume(id_, -1); }

    void SetVolume(int volume) { Mix_Volume(id_, volume); }

    bool Halt() { return Mix_HaltChannel(id_) == 0; }

    void Expire(int ticks) { Mix_ExpireChannel(id_, ticks); }

    bool FadeOut(int ms) { return Mix_FadeOutChannel(id_, ms) == 0; }

    Mix_Fading GetMusicFadingType() { return Mix_FadingChannel(id_); }

    void Pause() { Mix_Pause(id_); }
    void Resume() { Mix_Resume(id_); }
    bool Paused() { return Mix_Paused(id_) == 1; }

    bool IsPlaying() { return Mix_Playing(id_) != 0; }

    Mix_Chunk *GetChunk() { return Mix_GetChunk(id_); }

private:
    int id_{-1};
};

class MixChannelManager {
public:
    int  AllocateChannels(int num_chans);
    int  GetChannelsCount();
    int  ReserveChannels(int num);
    bool GroupChannel(int which, int tag);
    bool GroupChannels(int from, int to, int tag);
    /**
     *
     * @param tag
     * @return first available channel, or -1 if none are available.
     */
    int  GroupAvailable(int tag);

    int GroupCount(int tag);

    int GroupOldest(int tag);

    int GroupNewer(int tag);

    /**
     *
     * @param channel If a specific channel was requested,
     *                  and there is a chunk already playing there,
     *                  that chunk will be halted and the new chunk will take its place.
     * @param chunk
     * @param loops
     * @return
     */
    bool Play(int channel, std::shared_ptr<MixChunk> chunk, int loops) {
        return Mix_PlayChannel(channel, chunk->Get(), loops) != -1;
    }
    bool Play(int channel, std::shared_ptr<MixChunk> chunk, int loops, int ticks) {
        return Mix_PlayChannelTimed(channel, chunk->Get(), loops, ticks) != -1;
    }

    bool FadeIn(int channel, std::shared_ptr<MixChunk> chunk, int loops, int ms) {
        return Mix_FadeInChannel(channel, chunk->Get(), loops, ms) != -1;
    }

    bool FadeInTimed(int channel, std::shared_ptr<MixChunk> chunk, int loops, int ms, int ticks) {
        return Mix_FadeInChannelTimed(channel, chunk->Get(), loops, ms, ticks) != -1;
    }

    int GetVolume(int channel) { return Mix_Volume(channel, -1); }

    void SetVolume(int channel, int volume) { Mix_Volume(channel, volume); }

    int MasterVolume() { return Mix_MasterVolume(-1); }

    void SetMasterVolume(int volume) { Mix_MasterVolume(volume); }

    bool HaltChannel(int channel) { return Mix_HaltChannel(channel) == 0; }

    void HaltGroup(int tag) { Mix_HaltGroup(tag); }

    int FadeOutGroup(int tag, int ms) { return Mix_FadeOutGroup(tag, ms); }

    void Pause(int channel) { Mix_Pause(channel); }
    void Resume(int channel) { Mix_Resume(channel); }

    /**
     *
     * @param channel_id the channel to query, or -1 to query all channels.
     * @return 1 if channel paused, 0 otherwise. If `channel` is -1, returns the
     *         number of paused channels.
     */
    int Paused(int channel) { return Mix_Paused(channel); }

    /**
     *
     * @param channel
     * @return non-zero if channel is playing, zero otherwise. If `channel` is
     *          -1, return the total number of channel playings.
     */
    int Playing(int channel) { return Mix_Playing(channel); }

    MixChannel GetChannel(int channel_id) { return channels_[channel_id]; }

    std::function<void(int)> channel_finished_cb;

    static MixChannelManager *GetInstance() {
        static MixChannelManager instance_;
        return &instance_;
    }

    ~MixChannelManager() = default;

private:
    MixChannelManager() { Mix_ChannelFinished(&channel_Finished_callback); }

    static void channel_Finished_callback(int channel) {
        auto this_ = GetInstance();
        if(this_->channel_finished_cb) {
            this_->channel_finished_cb(channel);
        }
    }

    std::vector<MixChannel> channels_;
};


class MixMusic {
public:
    static std::shared_ptr<MixMusic> Load(std::string_view file) { return MixMusic::Create(Mix_LoadMUS(file.data())); }

    Mix_Music       *Get() { return music_; }
    const Mix_Music *Get() const { return music_; }

    bool Valid() const { return music_ != nullptr; }

    using MusicType = Mix_MusicType;

    MusicType GetType() const { return music_ ? Mix_GetMusicType(music_) : MusicType::MUS_NONE; }

    std::string GetTitle() const { return music_ ? Mix_GetMusicTitle(music_) : ""; }

    std::string GetTitleTag() const { return Mix_GetMusicTitleTag(this->music_); }

    std::string GetArtistTag() const { return Mix_GetMusicArtistTag(music_); }

    std::string GetAlbumTag() const { return Mix_GetMusicAlbumTag(music_); }

    std::string GetCopyrightTag() const { return Mix_GetMusicCopyrightTag(music_); }

    int GetVolume() const { return Mix_GetMusicVolume(music_); }

    double GetPosition() { return Mix_GetMusicPosition(music_); }

    double GetDuration() { return Mix_MusicDuration(music_); }

    double GetLoopStartTime() { return Mix_GetMusicLoopStartTime(music_); }

    double GetLoopEndTime() { return Mix_GetMusicLoopEndTime(music_); }

    double GetLoopLengthTime() { return Mix_GetMusicLoopLengthTime(music_); }

    ~MixMusic() {
        if(music_)
            Mix_FreeMusic(music_);
    }

private:
    static std::shared_ptr<MixMusic> Create(Mix_Music *&&music) {
        return std::shared_ptr<MixMusic>(new MixMusic(std::move(music)));
    }

    explicit MixMusic(Mix_Music *&&music) : music_(music) {}

    Mix_Music *music_{nullptr};
};

class MixMusicManager {
public:
    /**
     * 这将安排音乐对象开始混合以进行播放。
     *
     * 一次只能播放一个音乐对象;如果在播放另一个音乐对象时调用此函数，则当前播放的音乐将停止，新音乐将替换它。
     *
     * 请注意，如果当前播放的音乐正在淡出（通过Mix_FadeOutMusic（）），
     * 此功能将阻止，直到淡入淡出完成。如果需要避免这种情况，请务必在开始新音乐之前调用Mix_HaltMusic（）。
     *
     * @param music
     * @param loops
     * @return
     */
    bool Play(std::shared_ptr<MixMusic> music, int loops) { return Mix_PlayMusic(music->Get(), loops) == 0; }

    /**
     * 这将开始播放新音乐，与Mix_PlayMusic（）非常相似，
     * 但将以静音方式开始播放音乐，并在指定的毫秒数内淡入正常音量。
     *
     * 如果已经有音乐在播放，则该音乐将停止，新的音乐对象将取而代之。
     *
     * 如果大于零，则多次循环播放音乐。如果为 -1，则“无限”循环（~65000 次）。
     *
     * 淡入淡出的音乐会逐渐改变它的音量，就好像在它上面调用了
     * Mix_VolumeMusic（）（也就是说：你可能不应该在淡入淡出的音乐上调用Mix_VolumeMusic（））。
     * @param music
     * @param loops
     * @param ms
     * @return
     */
    bool FadeInMusic(std::shared_ptr<MixMusic> music, int loops, int ms) {
        return Mix_FadeInMusic(music->Get(), loops, ms) == 0;
    }

    bool FadeInMusicPos(std::shared_ptr<MixMusic> music, int loops, int ms, double position) {
        return Mix_FadeInMusicPos(music->Get(), loops, ms, position) != -1;
    }

    int Volume() { return Mix_VolumeMusic(-1); }

    void SetVolume(int volume) { Mix_VolumeMusic(volume); }

    void Halt() { Mix_HaltMusic(); }

    bool FadeOut(int ms) { return Mix_FadeOutMusic(ms) != 0; }

    Mix_Fading GetMusicFadingType() { return Mix_FadingMusic(); }

    void Pause() { Mix_PauseMusic(); }

    bool Paused() { return Mix_PausedMusic() == 1; }

    void Resume() { Mix_ResumeMusic(); }

    /**
     * Rewind the music stream.
     *
     * This causes the currently-playing music to start mixing from the beginning
     * of the music, as if it were just started.
     *
     * It's a legal no-op to rewind the music stream when not playing.
     */
    void Replay() { Mix_RewindMusic(); }

    // Mix_ModMusicJumpToOrder

    bool SetPosition(double position_sec) { return Mix_SetMusicPosition(position_sec) == 0; }

    bool IsPlaying() { return Mix_PlayingMusic() != 0; }

    bool SetCMD(const char *command) { return Mix_SetMusicCMD(command) == 0; }


    std::function<void()> music_finished_cb;

    static MixMusicManager *GetInstance() {
        static MixMusicManager instance_;
        return &instance_;
    }

    ~MixMusicManager() = default;

private:
    MixMusicManager() { Mix_HookMusicFinished(&MixMusicManager::music_Finished_callback); }

    static void music_Finished_callback() {
        auto this_ = GetInstance();
        if(this_->music_finished_cb) {
            this_->music_finished_cb();
        }
    }
};

} // namespace sdlpp

#endif // SDL2PP_SDL2PP_SDL_SDLMIXER_HPP_
