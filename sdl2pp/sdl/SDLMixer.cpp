#include "SDLMixer.hpp"

#include <SDL_log.h>

namespace sdlpp {

bool SDLMixer::OpenAudio(int frequency, Uint16 format, int channels, int chunk_size) {
    opened_ = Mix_OpenAudio(frequency, format, channels, chunk_size) == 0;
    if(opened_) {
        auto count = Mix_GetNumChunkDecoders();
        chunk_decoders_.resize(count);
        for(int i = 0; i < count; ++i) {
            chunk_decoders_[i] = Mix_GetChunkDecoder(i);
        }

        count = Mix_GetNumMusicDecoders();
        music_decoders_.resize(count);
        for(int i = 0; i < count; ++i) {
            music_decoders_[i] = Mix_GetMusicDecoder(i);
        }
    }
    return opened_;
}

bool SDLMixer::QuerySpec(int *frequency, Uint16 *format, int *channels) {
    return !!Mix_QuerySpec(frequency, format, channels);
}

const std::vector<std::string> &SDLMixer::GetChunkDecoders() const { return this->chunk_decoders_; }

const std::vector<std::string> &SDLMixer::GetMusicDecoders() const { return this->music_decoders_; }

bool SDLMixer::HasChunkDecoder(std::string_view name) const { return Mix_HasChunkDecoder(name.data()); }

bool SDLMixer::HasMusicDecoder(std::string_view name) const { return Mix_HasMusicDecoder(name.data()); }

int MixChannelManager::AllocateChannels(int num_chans) {
    auto count = Mix_AllocateChannels(num_chans);

    this->channels_.clear();
    for(int i = 0; i < count; ++i) {
        this->channels_.emplace_back(i);
    }

    return count;
}
int  MixChannelManager::ReserveChannels(int num) { return Mix_ReserveChannels(num); }
bool MixChannelManager::GroupChannel(int which, int tag) { return Mix_GroupChannel(which, tag) != 0; }
bool MixChannelManager::GroupChannels(int from, int to, int tag) { return Mix_GroupChannels(from, to, tag) == 0; }
int  MixChannelManager::GroupAvailable(int tag) { return Mix_GroupAvailable(tag); }
int  MixChannelManager::GroupCount(int tag) { return Mix_GroupCount(tag); }
int  MixChannelManager::GroupOldest(int tag) { return Mix_GroupOldest(tag); }
int  MixChannelManager::GroupNewer(int tag) { return Mix_GroupNewer(tag); }
int  MixChannelManager::GetChannelsCount() { return Mix_AllocateChannels(-1); }

} // namespace sdlpp