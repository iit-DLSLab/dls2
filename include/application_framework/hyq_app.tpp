#ifndef HYQ_APP_TPP_LJR4CEH3
#define HYQ_APP_TPP_LJR4CEH3

template <typename layer_t>
bool HyQApp::addLayer(std::shared_ptr<layer_t> pLayer)
{
	static_assert
	(
		std::is_base_of<AppLayer, layer_t>::value,
		"Error: argument must point to an instance of AppLayer"
	);

	std::lock_guard<std::mutex> lock(this->layers_mutex);

	// loop through all the layers and check their types
	auto it = this->layers.begin();
	for(; it != this->layers.end(); it += 1)
	{
		// If a layer of this type is already active in the architecture
		if(std::dynamic_pointer_cast<layer_t>(*it))
		{
			return false;
		}
	}

	this->layers.push_back(pLayer);
	return true;
}

#endif /* end of include guard: HYQ_APP_TPP_LJR4CEH3 */
